#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>

template<typename TResource>
class ResourceManager
{
public:

   ResourceManager() = default;
   ~ResourceManager() = default;

   ResourceManager(ResourceManager&) = delete;
   ResourceManager& operator=(ResourceManager&) = delete;

   ResourceManager(ResourceManager&&) = default;
   ResourceManager& operator=(ResourceManager&&) = default;

   template<typename TResourceLoader, typename... Args>
   std::shared_ptr<TResource> loadResource(const std::string& resourceID, Args&&... args);

   template<typename TResourceLoader, typename... Args>
   std::shared_ptr<TResource> loadUnmanagedResource(Args&&... args) const;

   std::shared_ptr<TResource> getResource(const std::string& resourceID) const;

   bool                       containsResource(const std::string& resourceID) const noexcept;

   void                       stopManagingResource(const std::string& resourceID) noexcept;
   void                       stopManagingAllResources() noexcept;

private:

   std::unordered_map<std::string, std::shared_ptr<TResource>> resources;
};

template<typename TResource>
template<typename TResourceLoader, typename... Args>
std::shared_ptr<TResource> ResourceManager<TResource>::loadResource(const std::string& resourceID, Args&&... args)
{
   std::shared_ptr<TResource> resource{};

   auto it = resources.find(resourceID);
   if (it == resources.cend())
   {
      // TODO: Are we taking advantage of move semantics inside this function?
      resource = TResourceLoader{}.loadResource(std::forward<Args>(args)...);
      if (resource)
      {
         resources[resourceID] = resource;
      }
   }
   else
   {
      std::cout << "Warning - ResourceManager::loadResource - A resource with the following ID already exists: " << resourceID << "\n";
      // TODO: What happens when I assign a value to a shared_ptr that already wraps a nullptr?
      resource = it->second;
   }

   return resource;
}

template<typename TResource>
template<typename TResourceLoader, typename... Args>
std::shared_ptr<TResource> ResourceManager<TResource>::loadUnmanagedResource(Args&&... args) const
{
   return TResourceLoader{}.loadResource(std::forward<Args>(args)...);
}

template<typename TResource>
std::shared_ptr<TResource> ResourceManager<TResource>::getResource(const std::string& resourceID) const
{
   auto it = resources.find(resourceID);
   if (it != resources.end())
   {
      return it->second;
   }
   else
   {
      std::cout << "Error - ResourceManager::getResource - A resource with the following ID does not exist: " << resourceID << "\n";
      return nullptr;
   }
}

template<typename TResource>
bool ResourceManager<TResource>::containsResource(const std::string& resourceID) const noexcept
{
   return (resources.find(resourceID) != resources.cend());
}

template<typename TResource>
void ResourceManager<TResource>::stopManagingResource(const std::string& resourceID) noexcept
{
   auto it = resources.find(resourceID);
   if (it != resources.end())
   {
      resources.erase(it);
   }
   else
   {
      std::cout << "Error - ResourceManager::stopManagingResource - A resource with the following ID does not exist: " << resourceID << "\n";
   }
}

template<typename TResource>
void ResourceManager<TResource>::stopManagingAllResources() noexcept
{
   resources.clear();
}

#endif

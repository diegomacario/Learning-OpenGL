#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <memory>
#include <utility>
#include <type_traits>
#include <unordered_map>

#include "resource_loader.h"

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
   std::shared_ptr<TResource> handle{};

   auto it = resources.find(resourceID);
   if (it == resources.cend())
   {
      auto resource = TResourceLoader{}.loadResource(std::forward<Args>(args)...);
      if (resource)
      {
         resources[resourceID] = resource;
         handle = std::move(resource);
      }
   }
   else
   {
      handle = it->second;
   }

   return handle;
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
   return (it == resources.end() ? nullptr : it->second);
}

template<typename TResource>
bool ResourceManager<TResource>::containsResource(const std::string& resourceID) const noexcept
{
   return (resources.find(resourceID) != resources.cend());
}

template<typename TResource>
void ResourceManager<TResource>::stopManagingResource(const std::string& resourceID) noexcept
{
   if (auto it = resources.find(resourceID); it != resources.end())
   {
      resources.erase(it);
   }
}

template<typename TResource>
void ResourceManager<TResource>::stopManagingAllResources() noexcept
{
   resources.clear();
}

#endif

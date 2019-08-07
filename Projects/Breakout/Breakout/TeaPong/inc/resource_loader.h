#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <memory>

template<typename TResourceLoaderDerived, typename TResource>
class ResourceLoader
{
public:

   ResourceLoader() = default;
   ~ResourceLoader() = default;

   ResourceLoader(ResourceLoader&) = delete;
   ResourceLoader& operator=(ResourceLoader&) = delete;

   ResourceLoader(ResourceLoader&&) = delete;
   ResourceLoader& operator=(ResourceLoader&&) = delete;

   template<typename... Args>
   std::shared_ptr<TResource> loadResource(Args&&... args) const
   {
      return static_cast<const TResourceLoaderDerived*>(this)->load(std::forward<Args>(args)...);
   }
};

#endif

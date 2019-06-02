#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <memory>

template<typename TResourceLoaderDerived, typename TResource>
class ResourceLoader
{
public:

   template<typename... Args>
   std::shared_ptr<TResource> loadResource(Args&&... args) const
   {
      return static_cast<const TResourceLoaderDerived*>(this)->load(std::forward<Args>(args)...);
   }
};

#endif

#ifndef ENTT_RESOURCE_LOADER_HPP
#define ENTT_RESOURCE_LOADER_HPP

#include <memory>
#include "fwd.hpp"

namespace entt {

 // Resource loaders must inherit from this class and stay true to the CRTP
 // idiom. Moreover, a resource loader must expose a public, const member
 // function named `load` that accepts a variable number of arguments and returns
 // a shared pointer to the resource just created.<br/>
 // As an example:
 //
 // @code{.cpp}
 // struct my_resource {};
 //
 // struct my_loader: entt::resource_loader<my_loader, my_resource> {
 //     std::shared_ptr<my_resource> load(int) const {
 //         // use the integer value somehow
 //         return std::make_shared<my_resource>();
 //     }
 // };
 // @endcode
 //
 // In general, resource loaders should not have a state or retain data of any
 // type. They should let the cache manage their resources instead.
template<typename Loader, typename Resource>
class resource_loader
{
    friend class resource_cache<Resource>; // resource_cache can now access the private interface of resource_handle.

    // Loads the resource and returns it. In case of errors, it returns an empty pointer.
    // Args - Types of arguments for the loader.
    // args - Arguments for the loader.
    template<typename... Args>
    std::shared_ptr<Resource> get(Args &&... args) const {
        return static_cast<const Loader *>(this)->load(std::forward<Args>(args)...);
    }
};

}

#endif // ENTT_RESOURCE_LOADER_HPP

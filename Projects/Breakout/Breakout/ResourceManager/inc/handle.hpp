#ifndef ENTT_RESOURCE_HANDLE_HPP
#define ENTT_RESOURCE_HANDLE_HPP

#include <memory>
#include <utility>
#include "config.h"
#include "fwd.hpp"

namespace entt {

 // A shared resource handle is a small class that wraps a resource and keeps it
 // alive even if it's deleted from the cache. It can be either copied or
 // moved. A handle shares a reference to the same resource with all the other
 // handles constructed for the same identifier.<br/>
 // As a rule of thumb, resources should never be copied nor moved. Handles are
 // the way to go to keep references to them.
template<typename Resource>
class resource_handle
{
    friend class resource_cache<Resource>; // resource_cache can now access the private interface of resource_handle.

    resource_handle(std::shared_ptr<Resource> res) ENTT_NOEXCEPT
        : resource{std::move(res)}
    {}

public:
    // Default constructor.
    resource_handle() ENTT_NOEXCEPT = default;

    // Gets a reference to the managed resource.
    // The behavior is undefined if the handle doesn't contain a resource.
    const Resource & get() const ENTT_NOEXCEPT
    {
        ENTT_ASSERT(static_cast<bool>(resource));
        return *resource;
    }

    // Casts a handle and gets a reference to the managed resource.
    // The behavior is undefined if the handle doesn't contain a resource.
    // This is what is referred to as a "conversion operator" that converts to a reference (https://stackoverflow.com/questions/1307876/how-do-conversion-operators-work-in-c)
    inline operator const Resource &() const ENTT_NOEXCEPT { return get(); }

    // Dereferences a handle to obtain the managed resource.
    // The behavior is undefined if the handle doesn't contain a resource.
    inline const Resource & operator *() const ENTT_NOEXCEPT { return get(); }

    // Gets a pointer to the managed resource from a handle.
    // The behavior is undefined if the handle doesn't contain a resource.
    // nullptr is returned if the handle doesn't contain a resource.
    inline const Resource * operator->() const ENTT_NOEXCEPT
    {
        ENTT_ASSERT(static_cast<bool>(resource));
        return resource.get();
    }

    // Returns true if a handle contains a resource, false otherwise.
    explicit operator bool() const { return static_cast<bool>(resource); }

private:
    std::shared_ptr<Resource> resource;
};


}

#endif // ENTT_RESOURCE_HANDLE_HPP

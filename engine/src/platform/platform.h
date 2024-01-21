#pragma once

#include "core/dyarr.h"

typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
struct VkAllocationCallbacks;

namespace nk {
    class Instance;

    class Platform {
    public:
        virtual ~Platform() = default;

        virtual bool pump_messages() = 0;
        virtual f64 get_absolute_time() = 0;
        virtual void sleep(u64 ms) = 0;

        virtual void get_required_extensions(Dyarr<cstr>& extensions) const = 0;
        virtual VkSurfaceKHR create_surface(Instance& instance, VkAllocationCallbacks* allocator) const = 0;

    protected:
        Platform(str application_name, const u32 x, const u32 y, const u32 width, const u32 height) {}
    };
}

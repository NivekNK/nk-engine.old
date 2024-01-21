#pragma once

#include "renderer/vulkan.h"

#include "platform/platform.h"
#include "memory/malloc_allocator.h"
#include "core/dyarr.h"

namespace nk {
    class Instance {
    public:
        Instance(Platform& platform, str application_name, VkAllocationCallbacks* allocator);
        ~Instance();

        Instance(const Instance&) = delete;
        Instance& operator=(const Instance&) = delete;

        inline VkInstance operator()() { return m_instance; }

    private:
        void create_instance(str application_name);
#if defined(NK_DEBUG)
        void create_debug_messenger();
#endif

        Platform& m_platform;

        MallocAllocator m_array_allocator;
        Dyarr<cstr> m_extensions;

        VkAllocationCallbacks* m_allocator;
        VkInstance m_instance;
#if defined(NK_DEBUG)
        VkDebugUtilsMessengerEXT m_debug_messenger;
#endif
    };
}

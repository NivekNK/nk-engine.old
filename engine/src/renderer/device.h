#pragma once

#include "renderer/vulkan.h"

#include "platform/platform.h"
#include "renderer/instance.h"
#include "core/arr.h"
#include "core/dyarr.h"

namespace nk {
    struct SwapchainSupportInfo {
        VkSurfaceCapabilitiesKHR capabilities;
        Arr<VkSurfaceFormatKHR> formats;
        Arr<VkPresentModeKHR> present_modes;
    };

    struct PhysicalDeviceRequirements {
        bool graphics;
        bool present;
        bool compute;
        bool transfer;
        bool sampler_anisotropy;
        bool discrete_gpu;
        Dyarr<cstr> extensions;
    };

    struct PhysicalDeviceQueueFamilyInfo {
        u32 graphics_family_index;
        u32 present_family_index;
        u32 compute_family_index;
        u32 transfer_family_index;
    };

    class Device {
    public:
        Device(Platform& platform, Instance& instance, VkAllocationCallbacks* allocator);
        ~Device();

        Device(const Device&) = delete;
        Device& operator=(const Device&) = delete;

    private:
        void select_physical_device(Allocator* allocator);
        void create_logical_device();
        void create_command_pool();

        void query_swapchain_support(
            SwapchainSupportInfo* out_support_info,
            VkPhysicalDevice physical_device,
            Allocator* allocator) const;

        bool physical_device_meets_requirements(
            PhysicalDeviceQueueFamilyInfo* out_queue_family,
            VkPhysicalDevice physical_device,
            const VkPhysicalDeviceProperties& properties,
            const VkPhysicalDeviceFeatures& features,
            const PhysicalDeviceRequirements& requirementst,
            Allocator* allocator) const;

        VkAllocationCallbacks* m_allocator;
        Instance& m_instance;

        VkSurfaceKHR m_surface;
        VkPhysicalDevice m_physical_device;
        PhysicalDeviceQueueFamilyInfo m_queue_family;
        VkPhysicalDeviceProperties m_properties;
        VkPhysicalDeviceFeatures m_features;
        VkPhysicalDeviceMemoryProperties m_memory;
    };
}

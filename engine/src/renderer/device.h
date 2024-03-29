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

        inline VkDevice operator()() { return m_logical_device; }
        inline VkDevice get() { return m_logical_device; }

        const SwapchainSupportInfo& get_swapchain_support_info() const { return m_swapchain_support_info; }
        VkSurfaceKHR get_surface() { return m_surface; }
        const PhysicalDeviceQueueFamilyInfo& get_queue_family_info() const { return m_queue_family; }
        const VkFormat get_depth_format() const { return m_depth_format; }

        bool find_memory_index(
            u32& out_memory_index,
            const u32 type_filter,
            VkMemoryPropertyFlags property_flags) const;

    private:
        void select_physical_device(Allocator* defer);
        void detect_depth_format();
        void create_logical_device();
        void create_command_pool();

        void query_swapchain_support(VkPhysicalDevice physical_device);

        bool physical_device_meets_requirements(
            PhysicalDeviceQueueFamilyInfo* out_queue_family,
            VkPhysicalDevice physical_device,
            const VkPhysicalDeviceProperties& properties,
            const VkPhysicalDeviceFeatures& features,
            const PhysicalDeviceRequirements& requirementst,
            Allocator* defer);

        VkAllocationCallbacks* m_allocator;
        Instance& m_instance;

        VkSurfaceKHR m_surface;

        VkPhysicalDevice m_physical_device;
        PhysicalDeviceQueueFamilyInfo m_queue_family;
        VkPhysicalDeviceProperties m_properties;
        VkPhysicalDeviceFeatures m_features;
        VkPhysicalDeviceMemoryProperties m_memory;

        VkDevice m_logical_device;
        VkQueue m_graphics_queue;
        VkQueue m_present_queue;
        VkQueue m_transfer_queue;

        VkCommandPool m_graphics_command_pool;

        Allocator* m_swapchain_support_info_allocator;
        SwapchainSupportInfo m_swapchain_support_info;

        VkFormat m_depth_format;
    };
}

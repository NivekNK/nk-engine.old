#pragma once

#include "renderer/vulkan.h"

#include "renderer/device.h"

namespace nk {
    struct VulkanImageCreateInfo {
        VkImageType image_type;
        VkExtent2D extents;
        VkFormat format;
        VkImageTiling tiling;
        VkImageUsageFlags usage;
        VkMemoryPropertyFlags memory_flags;
        bool create_view = false;
        VkImageAspectFlags view_aspect_flags = 0;
    };

    class Image {
    public:
        Image() = default;
        inline ~Image() { destroy(); }

        void init(const VulkanImageCreateInfo& create_info, Device* device, VkAllocationCallbacks* allocator);
        void destroy();

        void create_view(VkFormat format, VkImageAspectFlags aspect_flags);
        void transition_layout(VkImageLayout old_layout, VkImageLayout new_layout, VkCommandBuffer command_buffer);
        void copy_from_buffer(VkBuffer buffer, VkCommandBuffer command_buffer);

        inline VkImageView get_view() const { return m_view; }

    private:
        VkAllocationCallbacks* m_allocator;
        Device* m_device;

        VkImage m_image;
        VkDeviceMemory m_memory;
        VkImageView m_view;
        VkExtent2D m_extents;
        VkFormat m_format;
    };
}

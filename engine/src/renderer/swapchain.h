#pragma once

#include "renderer/vulkan.h"

#include "renderer/device.h"
#include "core/arr.h"
#include "renderer/image.h"
#include "renderer/framebuffer.h"

namespace nk {
    class RenderPass;

    class Swapchain {
    public:
        Swapchain(const u16 width, const u16 height, Device& device, VkAllocationCallbacks* allocator);
        ~Swapchain();

        const VkSurfaceFormatKHR& get_image_format() const { return m_image_format; }

        void regenerate_framebuffers(const u16 width, const u16 height, RenderPass* render_pass);

    private:
        void create_swapchain(const u16 width, const u16 height);
        void destroy_swapchain();

        VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, const u16 width, const u16 height);
        VkSurfaceFormatKHR choose_swap_surface_format(const Arr<VkSurfaceFormatKHR>& available_formats);
        VkPresentModeKHR choose_swap_present_mode(const Arr<VkPresentModeKHR>& available_present_modes);

        Device& m_device;
        VkAllocationCallbacks* m_allocator;

        u32 m_max_frames_in_flight;
        VkSwapchainKHR m_swapchain;
        u32 m_current_frame;

        Allocator* m_images_allocator;
        Arr<VkImage> m_images;
        Arr<VkImageView> m_views;

        Image m_depth_attachment;
        VkSurfaceFormatKHR m_image_format;
        VkExtent2D m_swapchain_extent;

        Allocator* m_framebuffers_allocator;
        Arr<Framebuffer> m_framebuffers;
    };
}

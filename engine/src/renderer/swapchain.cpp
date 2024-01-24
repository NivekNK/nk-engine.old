#include "nkpch.h"

#include "renderer/swapchain.h"

#include "renderer/render_pass.h"

namespace nk {
    Swapchain::Swapchain(const u16 width, const u16 height, Device& device, VkAllocationCallbacks* allocator)
        : m_device{device},
          m_allocator{allocator} {
        m_images_allocator = new MallocAllocator("SwapchainImages", MemoryType::Renderer);
        m_framebuffers_allocator = new MallocAllocator("Framebuffers", MemoryType::Renderer);
        create_swapchain(width, height);
        TraceLog("nk::Swapchain created.");
    }

    Swapchain::~Swapchain() {
        for (auto& framebuffer : m_framebuffers) {
            framebuffer.destroy();
        }
        m_framebuffers.free(m_framebuffers_allocator);
        delete m_framebuffers_allocator;
        InfoLog("Vulkan framebuffers destroyed.");

        destroy_swapchain();
        delete m_images_allocator;

        TraceLog("nk::Swapchain destroyed.");
    }

    void Swapchain::regenerate_framebuffers(const u16 width, const u16 height, RenderPass* render_pass) {
        const u32 image_count = m_images.length();
        if (m_framebuffers.empty()) {
            Framebuffer* framebuffers = m_framebuffers_allocator->allocate<Framebuffer>(image_count);
            m_framebuffers.init(framebuffers, image_count);
        }

        for (u32 i = 0; i < image_count; ++i) {
            // TODO: make this dynamic based on the currently configured attachments
            Arr<VkImageView> attachments = {{m_views[i], m_depth_attachment.get_view()}, m_framebuffers_allocator, false};
            m_framebuffers[i].init(width, height, attachments, &m_device, render_pass, m_allocator);
        }
    }

    void Swapchain::create_swapchain(const u16 width, const u16 height) {
        const SwapchainSupportInfo& swapchain_support_info = m_device.get_swapchain_support_info();

        VkSurfaceFormatKHR image_format = choose_swap_surface_format(swapchain_support_info.formats);
        VkPresentModeKHR present_mode = choose_swap_present_mode(swapchain_support_info.present_modes);
        VkExtent2D swapchain_extent = choose_swap_extent(swapchain_support_info.capabilities, width, height);

        u32 image_count = swapchain_support_info.capabilities.minImageCount + 1;
        if (swapchain_support_info.capabilities.maxImageCount > 0 &&
            image_count > swapchain_support_info.capabilities.maxImageCount) {
            image_count = swapchain_support_info.capabilities.maxImageCount;
        }

        m_max_frames_in_flight = image_count - 1;

        // Swapchain create info
        VkSwapchainCreateInfoKHR swapchain_create_info = {VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
        swapchain_create_info.surface = m_device.get_surface();
        swapchain_create_info.minImageCount = image_count;
        swapchain_create_info.imageFormat = image_format.format;
        swapchain_create_info.imageColorSpace = image_format.colorSpace;
        swapchain_create_info.imageExtent = swapchain_extent;
        swapchain_create_info.imageArrayLayers = 1;
        swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // Setup the queue family indices
        const PhysicalDeviceQueueFamilyInfo& queue_family = m_device.get_queue_family_info();
        if (queue_family.graphics_family_index != queue_family.present_family_index) {
            u32 queue_family_indices[] = {
                queue_family.graphics_family_index,
                queue_family.present_family_index
            };
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            swapchain_create_info.queueFamilyIndexCount = 2;
            swapchain_create_info.pQueueFamilyIndices = queue_family_indices;
        } else {
            swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            swapchain_create_info.queueFamilyIndexCount = 0;
            swapchain_create_info.pQueueFamilyIndices = nullptr;
        }

        swapchain_create_info.preTransform = swapchain_support_info.capabilities.currentTransform;
        swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchain_create_info.presentMode = present_mode;
        swapchain_create_info.clipped = VK_TRUE;
        swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

        VulkanCheck(vkCreateSwapchainKHR(m_device(), &swapchain_create_info, m_allocator, &m_swapchain));

        // Start with a zero frame index.
        m_current_frame = 0;

        // Images
        VulkanCheck(vkGetSwapchainImagesKHR(m_device(), m_swapchain, &image_count, nullptr));
        m_images.init(m_images_allocator->allocate<VkImage>(image_count), image_count);
        VulkanCheck(vkGetSwapchainImagesKHR(m_device(), m_swapchain, &image_count, m_images.data()));

        // Views
        m_views.init(m_images_allocator->allocate<VkImageView>(image_count), image_count);
        for (u32 i = 0; i < image_count; ++i) {
            VkImageViewCreateInfo view_info = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
            view_info.image = m_images[i];
            view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            view_info.format = image_format.format;
            view_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            view_info.subresourceRange.baseMipLevel = 0;
            view_info.subresourceRange.levelCount = 1;
            view_info.subresourceRange.baseArrayLayer = 0;
            view_info.subresourceRange.layerCount = 1;

            VulkanCheck(vkCreateImageView(m_device(), &view_info, m_allocator, &m_views[i]));
        }

        // Depth resources
        const VkFormat depth_format = m_device.get_depth_format();
        if (depth_format == VK_FORMAT_UNDEFINED) {
            FatalLog("Failed to find a supported depth format!");
        }

        // Create depth image and its view.
        auto depth_create_info = VulkanImageCreateInfo {
            .image_type = VK_IMAGE_TYPE_2D,
            .extents = {width, height},
            .format = depth_format,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
            .memory_flags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            .create_view = true,
            .view_aspect_flags = VK_IMAGE_ASPECT_DEPTH_BIT,
        };
        // m_depth_attachment = m_images_allocator->construct<Image>(depth_create_info, &m_device, m_allocator);
        m_depth_attachment.init(depth_create_info, &m_device, m_allocator);
        m_image_format = image_format;
        m_swapchain_extent = swapchain_extent;
    }

    void Swapchain::destroy_swapchain() {
        // Only destroy the views, not the images, since those are owned by the swapchain and are thus
        // destroyed when it is.
        for (u32 i = 0; i < m_views.length(); ++i) {
            vkDestroyImageView(m_device(), m_views[i], m_allocator);
        }
        m_views.free(m_images_allocator);

        vkDestroySwapchainKHR(m_device(), m_swapchain, m_allocator);
        m_images.free(m_images_allocator);

        m_depth_attachment.destroy();
        DebugLog("Depth free");
        // m_images_allocator->destroy<Image>(m_depth_attachment);
    }

    VkExtent2D Swapchain::choose_swap_extent(const VkSurfaceCapabilitiesKHR& capabilities, const u16 width, const u16 height) {
        if (capabilities.currentExtent.width != u32_max)
            return capabilities.currentExtent;

        VkExtent2D current_extent = {width, height};
        current_extent.width = MaxAB(
            capabilities.minImageExtent.width,
            MinAB(capabilities.maxImageExtent.width, current_extent.width));
        current_extent.height = MaxAB(
            capabilities.minImageExtent.height,
            MinAB(capabilities.maxImageExtent.height, current_extent.height));

        return current_extent;
    }

    VkSurfaceFormatKHR Swapchain::choose_swap_surface_format(const Arr<VkSurfaceFormatKHR>& available_formats) {
        for (const auto& available_format : available_formats) {
            if (available_format.format == VK_FORMAT_B8G8R8A8_UNORM &&
                available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return available_format;
            }
        }
        return available_formats[0];
    }

    VkPresentModeKHR Swapchain::choose_swap_present_mode(const Arr<VkPresentModeKHR>& available_present_modes) {
        for (const auto& available_present_mode : available_present_modes) {
            if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
                return available_present_mode;
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }
}

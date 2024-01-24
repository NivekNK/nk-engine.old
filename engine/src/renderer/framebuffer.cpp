#include "nkpch.h"

#include "renderer/framebuffer.h"

#include "renderer/render_pass.h"

namespace nk {
    void Framebuffer::init(const u16 width,
                           const u16 height,
                           Arr<VkImageView>& attachments,
                           Device* device,
                           RenderPass* render_pass,
                           VkAllocationCallbacks* allocator) {
        m_device = device;
        m_render_pass = render_pass;
        m_allocator = allocator;
        m_attachments = std::move(attachments);

        // Creation info
        VkFramebufferCreateInfo framebuffer_create_info = {VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        framebuffer_create_info.renderPass = m_render_pass->get();
        framebuffer_create_info.attachmentCount = m_attachments.length();
        framebuffer_create_info.pAttachments = m_attachments.data();
        framebuffer_create_info.width = width;
        framebuffer_create_info.height = height;
        framebuffer_create_info.layers = 1;

        VulkanCheck(vkCreateFramebuffer(m_device->get(), &framebuffer_create_info, m_allocator, &m_framebuffer));
    }

    void Framebuffer::destroy() {
        if (m_framebuffer == nullptr)
            return;

        vkDestroyFramebuffer(m_device->get(), m_framebuffer, m_allocator);
        m_attachments.free();
        m_framebuffer = nullptr;
    }
}

#pragma once

#include "renderer/vulkan.h"

#include "core/arr.h"
#include "renderer/device.h"

namespace nk {
    class RenderPass;

    class Framebuffer {
    public:
        Framebuffer() = default;
        inline ~Framebuffer() { destroy(); }

        void init(const u16 width,
                  const u16 height,
                  Arr<VkImageView>& attachments,
                  Device* device,
                  RenderPass* render_pass,
                  VkAllocationCallbacks* allocator);
        void destroy();

        inline VkFramebuffer operator()() { return m_framebuffer; }

    private:
        VkAllocationCallbacks* m_allocator;
        Device* m_device;
        RenderPass* m_render_pass;

        VkFramebuffer m_framebuffer;
        Arr<VkImageView> m_attachments;
    };
}

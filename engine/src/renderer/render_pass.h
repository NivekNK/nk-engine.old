#pragma once

#include "renderer/vulkan.h"

#include "renderer/device.h"
#include "renderer/swapchain.h"

namespace nk {
   enum class RenderPassState {
        Ready,
        Recording,
        InRenderPass,
        RecordingEnded,
        Submitted,
        NotAllocated
    };

    struct RenderPassColor {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };

    struct RenderPassCreateInfo {
        VkRect2D render_area;
        RenderPassColor clear_color;
        f32 depth;
        u32 stencil;
    };

    class RenderPass {
    public:
        RenderPass(const RenderPassCreateInfo& create_info,
                   Device& device,
                   Swapchain& swapchain,
                   VkAllocationCallbacks* allocator);
        ~RenderPass();

        inline VkRenderPass operator()() { return m_render_pass; }
        inline VkRenderPass get() { return m_render_pass; }

    private:
        VkAllocationCallbacks* m_allocator;
        Device& m_device;

        VkRenderPass m_render_pass;

        VkRect2D m_render_area;
        RenderPassColor m_clear_color;
        f32 m_depth;
        u32 m_stencil;

        RenderPassState m_state;
    };
}

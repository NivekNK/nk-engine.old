#pragma once

#include "nk/renderer.h"

#include "platform/platform.h"
#include "renderer/instance.h"
#include "renderer/device.h"
#include "renderer/swapchain.h"
#include "renderer/render_pass.h"

namespace nk {
    class Window;

    class RendererBackend : public Renderer {
    public:
        RendererBackend(str application_name, Window& window, Platform& platform);
        virtual ~RendererBackend() override;

        virtual bool draw_frame(const RenderPacket& packet) override;

    private:
        bool begin_frame(f32 delta_time);
        bool end_frame(f32 delta_time);

        Window& m_window;

        VkAllocationCallbacks* m_vulkan_allocator = nullptr;
        u32 m_frame_number = 0;

        Allocator* m_allocator;
        Instance* m_instance;
        Device* m_device;
        Swapchain* m_swapchain;
        RenderPass* m_main_render_pass;
    };
}

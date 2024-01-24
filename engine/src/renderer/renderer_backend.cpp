#include "nkpch.h"

#include "renderer/renderer_backend.h"

#include "nk/window.h"

namespace nk {
    RendererBackend::RendererBackend(str application_name, Window& window, Platform& platform)
        : m_window{window} {
        m_allocator = new MallocAllocator("RendererBackend", MemoryType::Renderer);

        m_instance = m_allocator->construct<Instance>(platform, application_name, m_vulkan_allocator);

        m_device = m_allocator->construct<Device>(platform, *m_instance, m_vulkan_allocator);

        m_swapchain = m_allocator->construct<Swapchain>(
            m_window.get_width(), m_window.get_height(), *m_device, m_vulkan_allocator);

        auto main_render_pass_create_info = RenderPassCreateInfo {
            .render_area = {{0, 0}, {m_window.get_width(), m_window.get_height()}},
            .clear_color = {0.0f, 0.0f, 0.2f, 1.0f},
            .depth = 1.0f,
            .stencil = 0,
        };
        m_main_render_pass = m_allocator->construct<RenderPass>(
            main_render_pass_create_info, *m_device, *m_swapchain, m_vulkan_allocator);

        m_swapchain->regenerate_framebuffers(m_window.get_width(), m_window.get_height(), m_main_render_pass);
        InfoLog("Vulkan framebuffers created.");

        TraceLog("nk::RendererBackend created.");
    }

    RendererBackend::~RendererBackend() {
        m_allocator->destroy<RenderPass>(m_main_render_pass);

        m_allocator->destroy<Swapchain>(m_swapchain);

        m_allocator->destroy<Device>(m_device);

        m_allocator->destroy<Instance>(m_instance);

        delete m_allocator;
        TraceLog("nk::RendererBackend destroyed.");
    }

    bool RendererBackend::draw_frame(const RenderPacket& packet) {
        if (begin_frame(packet.delta_time)) {
            if (!end_frame(packet.delta_time)) {
                ErrorLog("RendererBackend::draw_frame failed!");
                return false;
            }
        }

        return true;
    }

    bool RendererBackend::begin_frame(f32 delta_time) {
        return true;
    }

    bool RendererBackend::end_frame(f32 delta_time) {
        m_frame_number++;
        return true;
    }
}

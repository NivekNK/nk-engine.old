#include "nkpch.h"

#include "renderer/renderer_backend.h"

#include "nk/window.h"

namespace nk {
    RendererBackend::RendererBackend(str application_name, Window& window, Platform& platform)
        : m_window{window} {
        m_allocator = new MallocAllocator("RendererBackend", MemoryType::Renderer);

        m_instance = m_allocator->construct<Instance>(platform, application_name, m_vulkan_allocator);
        m_device = m_allocator->construct<Device>(platform, *m_instance, m_vulkan_allocator);

        TraceLog("nk::RendererBackend created.");
    }

    RendererBackend::~RendererBackend() {
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

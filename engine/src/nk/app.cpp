#include "nkpch.h"

#include "nk/app.h"

#include "memory/allocator.h"
#include "nk/system_manager.h"

#if defined(NK_PLATFORM_WINDOWS)
    #include "platform/platform_win32.h"
#elif defined(NK_PLATFORM_LINUX)
    #error Not implemented yet!
#endif

namespace nk {
    App* App::s_instance = nullptr;

    App::App(const ApplicationConfig& config)
        : m_window{config} {
        Assert(s_instance == nullptr);
        s_instance = this;

        m_running = true;

        Allocator* allocator = SystemManager::get_system_allocator();
#if defined(NK_PLATFORM_WINDOWS)
        m_platform = allocator->construct<PlatformWin32>(
            config.name,
            config.start_pos_x,
            config.start_pos_y,
            config.start_width,
            config.start_height
        );
#elif defined(NK_PLATFORM_LINUX)
        #error Not implemented yet!
#endif
    }

    void App::run() {
        while(m_running) {
            if (!m_platform->pump_messages()) {
                m_running = false;
            }

            if (!m_window.is_suspended()) {
                if (!m_window.update(0.0f)) {
                    FatalLog("Window update failed. shutting down!");
                    break;
                }

                if (!m_window.render(0.0f)) {
                    FatalLog("Window render failed. shutting down!");
                    break;
                }
            }
        }

        m_running = false;

        Allocator* allocator = SystemManager::get_system_allocator();
        allocator->destroy(m_platform);
    }
}

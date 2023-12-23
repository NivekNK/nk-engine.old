#include "nkpch.h"

#include "nk/app.h"

#include "memory/malloc_allocator.h"
#include "system/input.h"
#include "event/window_event.h"
#include "event/key_event.h"

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

        m_event_callback = [this](Event& event) {
            this->on_event(event);
        };

        m_key_callback = [this](KeyEvent& event) {
            this->on_key(event);
        };

        m_event_dispatcher.add_listener<WindowCloseEvent>(BindFunc(App::on_application_close));

        m_allocator = new MallocAllocator("App", MemoryType::Application);
#if defined(NK_PLATFORM_WINDOWS)
        m_platform = m_allocator->construct<PlatformWin32>(
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

                InputSystem::get().update(0.0f);
            }
        }

        m_running = false;

        m_allocator->destroy<PlatformWin32>(m_platform);
        delete m_allocator;
    }

    void App::on_event(Event& event) {
        m_event_dispatcher.dispatch<WindowCloseEvent>(event);
    }

    void App::on_key(KeyEvent& event) {
        if (event.get_event_type() == EventType::KeyPressed &&
            event.keycode() == KeyCode::Escape) {
            WindowCloseEvent window_close;
            on_event(window_close);
            return;
        }

        DebugLog("{}", event);
    }

    void App::on_application_close(WindowCloseEvent& event) {
        m_running = false;
        DebugLog("{}: Closing application!", event);
    }
}

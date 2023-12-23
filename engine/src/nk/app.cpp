#include "nkpch.h"

#include "nk/app.h"

#include "memory/malloc_allocator.h"
#include "nk/clock.h"
#include "system/input.h"
#include "event/window_event.h"

#if defined(NK_PLATFORM_WINDOWS)
    #include "platform/platform_win32.h"
#elif defined(NK_PLATFORM_LINUX)
    #error Not implemented yet!
#endif

namespace nk {
    App* App::s_instance = nullptr;

    void App::run() {
        m_clock->start();
        m_clock->update();

        m_last_time = m_clock->elapsed();
        f64 running_time = 0.0f;
        u8 frame_count = 0.0f;
        f64 target_frame_seconds = 1.0f / 60;

        while(m_window.is_running()) {
            if (!m_platform->pump_messages()) {
                WindowCloseEvent event{};
                m_window.on_event(event);
            }

            if (!m_window.is_suspended()) {
                m_clock->update();
                f64 current_time = m_clock->elapsed();
                f64 delta_time = current_time - m_last_time;
                f64 frame_start_time = m_platform->get_absolute_time();

                if (!m_window.update(delta_time)) {
                    FatalLog("Window update failed. shutting down!");
                    break;
                }

                if (!m_window.render(delta_time)) {
                    FatalLog("Window render failed. shutting down!");
                    break;
                }

                f64 frame_end_time = m_platform->get_absolute_time();
                f64 frame_elapsed_time = frame_end_time - frame_start_time;
                running_time += frame_elapsed_time;

                f64 remaining_seconds = target_frame_seconds - frame_elapsed_time;
                if (remaining_seconds > 0) {
                    u64 remaining_ms = static_cast<u64>(remaining_seconds * 1000);
                    bool limit_frames = false;
                    if (remaining_ms > 0 && limit_frames) {
                        m_platform->sleep(remaining_ms - 1);
                    }
                    frame_count++;
                }

                InputSystem::update(delta_time);

                m_last_time = current_time;
            }
        }

        m_allocator->destroy<Clock>(m_clock);
        m_allocator->destroy<PlatformWin32>(m_platform);
        delete m_allocator;
    }

    void App::close() {
        WindowCloseEvent event{};
        s_instance->m_window.on_event(event);
    }

    void App::on_event(Event& event) {
        s_instance->m_window.on_event(event);
    }

    void App::on_key_event(KeyEvent& event) {
        s_instance->m_window.on_key_event(event);
    }

    App::App(const ApplicationConfig& config)
        : m_window{config} {
        Assert(s_instance == nullptr);
        s_instance = this;

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
        m_clock = m_allocator->construct<Clock>(*m_platform);
    }
}

#pragma once

#include "event/event_dispatcher.h"

namespace nk {
    struct ApplicationConfig;
    class KeyEvent;
    class WindowCloseEvent;

    class Window {
    public:
        Window(const ApplicationConfig& config);
        ~Window();

        bool update(f32 delta_time);
        bool render(f32 delta_time);

        void on_event(Event& event);
        void on_key_event(KeyEvent& event);

        inline bool is_suspended() const { return m_suspended; }
        inline bool is_running() const { return m_running; }

    private:
        void on_window_close(WindowCloseEvent& event);

        bool m_suspended;
        bool m_running;

        i16 m_pos_x;
        i16 m_pos_y;
        u16 m_width;
        u16 m_height;

        EventDispatcher m_event_dispatcher;
        EventDispatcher m_key_dispatcher;
    };
}

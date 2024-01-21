#include "nkpch.h"

#include "nk/window.h"

#include "nk/app.h"
#include "system/input.h"
#include "event/window_event.h"
#include "event/key_event.h"

namespace nk {
    Window::Window(const ApplicationConfig& config)
        : m_suspended{false},
          m_running{true},
          m_pos_x{config.start_pos_x},
          m_pos_y{config.start_pos_y},
          m_width{config.start_width},
          m_height{config.start_height} {
        m_event_dispatcher.add_listener<WindowCloseEvent>(BindFunc(Window::on_window_close));
    }

    Window::~Window() {
    }

    bool Window::update(f32 delta_time) {
        return true;
    }

    bool Window::render(f32 delta_time) {
        return true;
    }

    void Window::on_event(Event& event) {
        m_event_dispatcher.dispatch<WindowCloseEvent>(event);
    }

    void Window::on_key_event(KeyEvent& event) {
        if (event.get_event_type() == EventType::KeyPressed &&
            event.keycode() == KeyCode::Escape) {
            WindowCloseEvent window_close;
            on_event(window_close);
            return;
        }

        DebugLog("{}", event);
    }

    void Window::on_window_close(WindowCloseEvent& event) {
        m_running = false;
        DebugLog("{}: Closing application!", event);
    }

}

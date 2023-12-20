#include "nkpch.h"

#include "nk/window.h"

#include "nk/app.h"

#include "event/application_event.h"

namespace nk {
    void on_window_close(WindowCloseEvent& event) {
        event.handle();
        WarnLog("{}", event.is_handled());
    }

    Window::Window(const ApplicationConfig& config)
        : m_suspended{false},
          m_pos_x{config.start_pos_x},
          m_pos_y{config.start_pos_y},
          m_width{config.start_width},
          m_height{config.start_height} {
        m_dispatcher.add_listener<WindowCloseEvent>(on_window_close);
    }

    Window::~Window() {
        m_dispatcher.remove_listener<WindowCloseEvent>(m_test);
    }

    bool Window::update(f32 delta_time) {
        Event& event_test = m_close_test;
        m_dispatcher.dispatch<WindowCloseEvent>(event_test);
        return true;
    }

    bool Window::render(f32 delta_time) {
        return true;
    }

    void Window::on_resize(u16 width, u16 height) {
    }
}

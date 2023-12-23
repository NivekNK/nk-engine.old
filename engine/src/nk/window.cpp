#include "nkpch.h"

#include "nk/window.h"

#include "nk/app.h"

namespace nk {
    Window::Window(const ApplicationConfig& config)
        : m_suspended{false},
          m_pos_x{config.start_pos_x},
          m_pos_y{config.start_pos_y},
          m_width{config.start_width},
          m_height{config.start_height} {
    }

    Window::~Window() {
    }

    bool Window::update(f32 delta_time) {
        return true;
    }

    bool Window::render(f32 delta_time) {
        return true;
    }
}

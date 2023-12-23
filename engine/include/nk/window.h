#pragma once

#include "event/event_dispatcher.h"

namespace nk {
    struct ApplicationConfig;

    class Window {
    public:
        Window(const ApplicationConfig& config);
        ~Window();

        bool update(f32 delta_time);
        bool render(f32 delta_time);

        inline bool is_suspended() const { return m_suspended; }

    private:
        bool m_suspended;

        i16 m_pos_x;
        i16 m_pos_y;
        u16 m_width;
        u16 m_height;
    };
}

#pragma once

#include "event/event.h"

namespace nk {
    class WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() = default;

        DefineEventClassType(WindowClose)
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(const u16 width, const u16 height)
            : m_width{width}, m_height{height} {}

        u16 width() const { return m_width; }
        u16 height() const { return m_height; }

        virtual str to_string() const override {
            return std::format("{}: {}, {}", get_name(), m_width, m_height);
        }

        DefineEventClassType(WindowResize)

    private:
        u16 m_width;
        u16 m_height;
    };

    class WindowGainedFocusEvent : public Event {
    public:
        WindowGainedFocusEvent() = default;

        DefineEventClassType(WindowGainedFocus)
    };

    class WindowLostFocusEvent : public Event {
    public:
        WindowLostFocusEvent() = default;

        DefineEventClassType(WindowLostFocus)
    };

    class WindowMovedEvent : public Event {
    public:
        WindowMovedEvent(const i32 pos_x, const i32 pos_y)
            : m_pos_x{pos_x}, m_pos_y{pos_y} {}

        i32 x() { return m_pos_x; }
        i32 y() { return m_pos_y; }

        virtual str to_string() const override {
            return std::format("{}: {}, {}", get_name(), m_pos_x, m_pos_y);
        }

        DefineEventClassType(WindowMoved)

    private:
        i32 m_pos_x;
        i32 m_pos_y;
    };
}

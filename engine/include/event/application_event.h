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
}

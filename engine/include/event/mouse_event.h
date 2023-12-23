#pragma once

#include "event/event.h"
#include "nk/input_codes.h"

namespace nk {
    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(const i32 pos_x, const i32 pos_y)
            : m_pos_x{pos_x}, m_pos_y{pos_y} {}

        i32 x() const { return m_pos_x; }
        i32 y() const { return m_pos_y; }

        virtual str to_string() const override {
            return std::format("{}: {}, {}", get_name(), m_pos_x, m_pos_y);
        }

        DefineEventClassType(MouseMoved)

    private:
        i32 m_pos_x;
        i32 m_pos_y;
    };

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(const f32 z_delta, const i8 z_delta_normalized)
            : m_z_delta{z_delta}, m_z_delta_normalized{z_delta_normalized} {}

        f32 z() const { return m_z_delta; }
        i8 z_normalized() const { return m_z_delta_normalized; }

        virtual std::string to_string() const override {
            return std::format("{}: {} (normalized: {})", get_name(), m_z_delta, m_z_delta_normalized);
        }

        DefineEventClassType(MouseScrolled)

    private:
        f32 m_z_delta;
        i8 m_z_delta_normalized;
    };

    class MouseButtonEvent : public Event {
    public:
        MouseButton button() const { return m_button; }

        virtual str to_string() const override {
            return std::format("{}: {}", get_name(), static_cast<u8>(m_button));
        }

    protected:
        MouseButtonEvent(const MouseButton button)
            : m_button{button} {}

        MouseButton m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(const MouseButton button)
            : MouseButtonEvent(button) {}

        DefineEventClassType(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(const MouseButton button)
            : MouseButtonEvent(button) {}

        DefineEventClassType(MouseButtonReleased)
    };
}

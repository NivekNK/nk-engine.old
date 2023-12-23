#pragma once

#include "event/event.h"
#include "nk/input_codes.h"

namespace nk {
    class KeyEvent : public Event {
    public:
        KeyCodeFlag keycode() const { return m_keycode; }
        KeyCodeFlag mods() const { return m_mods; }

    protected:
        KeyEvent(const KeyCodeFlag keycode, const KeyCodeFlag mods)
            : m_keycode{keycode}, m_mods{mods} {}

        KeyCodeFlag m_keycode;
        KeyCodeFlag m_mods;
    };

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(const KeyCodeFlag keycode, const KeyCodeFlag mods, const u32 repeat_count)
            : KeyEvent(keycode, mods), m_repeat_count{repeat_count} {}

        u32 repeat_count() const { return m_repeat_count; }

        virtual std::string to_string() const override {
            return std::format("{}: '{}' ({} repeats)", get_name(), static_cast<char>(m_keycode), m_repeat_count);
        }

        DefineEventClassType(KeyPressed)

    private:
        u32 m_repeat_count;
    };

    class KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(const KeyCodeFlag keycode, const KeyCodeFlag mods)
            : KeyEvent(keycode, mods) {}

        virtual std::string to_string() const override {
            return std::format("{}: {}", get_name(), static_cast<char>(m_keycode));
        }

        DefineEventClassType(KeyReleased)
    };
}

#include "nkpch.h"

#include "system/input.h"

namespace nk {
    bool InputSystem::s_initialized = false;

    void InputSystem::update(f64 delta_time) {
        memcpy(&m_previous_keyboard_state, &m_current_keyboard_state, sizeof(KeyboardState));
        memcpy(&m_previous_mouse_state, &m_current_mouse_state, sizeof(MouseState));
    }

    void InputSystem::process_key(KeyCodeFlag keycode, bool pressed) {
        if (m_current_keyboard_state.keys[keycode] == pressed)
            return;

        m_current_keyboard_state.keys[keycode] = pressed;
    }

    void InputSystem::process_mouse_button(MouseButton button, bool pressed) {
        const u8 index = static_cast<u8>(button);
        if (m_current_mouse_state.buttons[index] == pressed)
            return;

        m_current_mouse_state.buttons[index] = pressed;
    }

    void InputSystem::process_mouse_move(i32 x, i32 y) {
        if (m_current_mouse_state.x == x && m_current_mouse_state.y == y)
            return;

        m_current_mouse_state.x = x;
        m_current_mouse_state.y = y;
    }

    bool InputSystem::is_key_down_impl(KeyCodeFlag keycode) {
        if (s_initialized)
            return false;
        return m_current_keyboard_state.keys[keycode] == true;
    }

    bool InputSystem::is_key_up_impl(KeyCodeFlag keycode) {
        if (s_initialized)
            return true;
        return m_current_keyboard_state.keys[keycode] == false;
    }

    bool InputSystem::was_key_down_impl(KeyCodeFlag keycode) {
        if (s_initialized)
            return false;
        return m_previous_keyboard_state.keys[keycode] == true;
    }

    bool InputSystem::was_key_up_impl(KeyCodeFlag keycode) {
        if (s_initialized)
            return true;
        return m_previous_keyboard_state.keys[keycode] == false;
    }

    bool InputSystem::is_mouse_button_down_impl(MouseButton button) {
        if (s_initialized)
            return false;
        return m_current_mouse_state.buttons[static_cast<u8>(button)] == true;
    }

    bool InputSystem::is_mouse_button_up_impl(MouseButton button) {
        if (s_initialized)
            return true;
        return m_current_mouse_state.buttons[static_cast<u8>(button)] == false;
    }

    bool InputSystem::was_mouse_button_down_impl(MouseButton button) {
        if (s_initialized)
            return false;
        return m_previous_mouse_state.buttons[static_cast<u8>(button)] == true;
    }

    bool InputSystem::was_mouse_button_up_impl(MouseButton button) {
        if (s_initialized)
            return true;
        return m_previous_mouse_state.buttons[static_cast<u8>(button)] == false;
    }

    void InputSystem::get_mouse_position_impl(i32& out_x, i32& out_y) {
        if (s_initialized)
            return;
        out_x = m_current_mouse_state.x;
        out_y = m_current_mouse_state.y;
    }

    void InputSystem::get_previous_mouse_position_impl(i32& out_x, i32& out_y) {
        if (s_initialized)
            return;
        out_x = m_previous_mouse_state.x;
        out_y = m_previous_mouse_state.y;
    }
}

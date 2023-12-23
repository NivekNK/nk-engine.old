#pragma once

#include "nk/input_codes.h"

namespace nk {
    class InputSystem {
    public:
        struct KeyboardState {
            bool keys[256];
        };

        struct MouseState {
            i32 x;
            i32 y;
            bool buttons[static_cast<u8>(MouseButton::MaxButtons)];
        };

        static void init() {
            s_initialized = true;
            get();
        }
        static void shutdown() {
            s_initialized = false;
        }

        static InputSystem& get() {
            static InputSystem instance;
            return instance;
        }

        ~InputSystem() = default;

        static bool is_key_down(KeyCodeFlag keycode) {
            return get().is_key_down_impl(keycode);
        }

        static bool is_key_up(KeyCodeFlag keycode) {
            return get().is_key_up(keycode);
        }

        static bool was_key_down(KeyCodeFlag keycode) {
            return get().was_key_down_impl(keycode);
        }

        static bool was_key_up(KeyCodeFlag keycode) {
            return get().was_key_up_impl(keycode);
        }

        static bool is_mouse_button_down(MouseButton button) {
            return get().is_mouse_button_down_impl(button);
        }

        static bool is_mouse_button_up(MouseButton button) {
            return get().is_mouse_button_up_impl(button);
        }

        static bool was_mouse_button_down(MouseButton button) {
            return get().was_mouse_button_down(button);
        }

        static bool was_mouse_button_up(MouseButton button) {
            return get().was_mouse_button_up_impl(button);
        }

        static void get_mouse_position(i32& out_x, i32& out_y) {
            get().get_mouse_position_impl(out_x, out_y);
        }

        static void get_previous_mouse_position(i32& out_x, i32& out_y) {
            get().get_previous_mouse_position_impl(out_x, out_y);
        }

        KeyboardState get_previous_keyboard_state() const { return m_previous_keyboard_state; }

        void update(f64 delta_time);
        void process_key(KeyCodeFlag keycode, bool pressed);
        void process_mouse_button(MouseButton button, bool pressed);
        void process_mouse_move(i32 x, i32 y);

    private:
        InputSystem() = default;

        bool is_key_down_impl(KeyCodeFlag keycode);
        bool is_key_up_impl(KeyCodeFlag keycode);
        bool was_key_down_impl(KeyCodeFlag keycode);
        bool was_key_up_impl(KeyCodeFlag keycode);

        bool is_mouse_button_down_impl(MouseButton button);
        bool is_mouse_button_up_impl(MouseButton button);
        bool was_mouse_button_down_impl(MouseButton button);
        bool was_mouse_button_up_impl(MouseButton button);

        void get_mouse_position_impl(i32& out_x, i32& out_y);
        void get_previous_mouse_position_impl(i32& out_x, i32& out_y);

        KeyboardState m_current_keyboard_state;
        KeyboardState m_previous_keyboard_state;

        MouseState m_current_mouse_state;
        MouseState m_previous_mouse_state;

        static bool s_initialized;
    };
}

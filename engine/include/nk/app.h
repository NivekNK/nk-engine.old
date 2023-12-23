#pragma once

#include "nk/window.h"

namespace nk {
    class Allocator;
    class Platform;
    class Event;
    class KeyEvent;
    class WindowCloseEvent;

    struct ApplicationConfig {
        str name;
        i16 start_pos_x;
        i16 start_pos_y;
        u16 start_width;
        u16 start_height;
    };

    class App {
    public:
        using EventCallback = std::function<void(Event&)>;
        using KeyEventCallback = std::function<void(KeyEvent&)>;

        virtual ~App() = default;

        void run();

        void on_event(Event& event);
        void on_key(KeyEvent& event);

        static App& get() { return *s_instance; }
        static void close() { get().m_running = false; }

        static EventCallback& get_event_callback() { return get().m_event_callback; }
        static KeyEventCallback& gey_key_callback() { return get().m_key_callback; }

    protected:
        App(const ApplicationConfig& config);

        void on_application_close(WindowCloseEvent& event);

        bool m_running;
        f64 m_last_time;

        EventDispatcher m_event_dispatcher;
        EventDispatcher m_key_dispatcher;
        EventCallback m_event_callback;
        KeyEventCallback m_key_callback;

        Allocator* m_allocator;

        Window m_window;
        Platform* m_platform;

    private:
        static App* s_instance;
    };
}

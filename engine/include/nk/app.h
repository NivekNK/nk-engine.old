#pragma once

#include "nk/window.h"

namespace nk {
    class Allocator;
    class Platform;
    class Clock;

    struct ApplicationConfig {
        str name;
        i16 start_pos_x;
        i16 start_pos_y;
        u16 start_width;
        u16 start_height;
    };

    class App {
    public:
        virtual ~App() = default;

        void run();

        static void close();
        static void on_event(Event& event);
        static void on_key_event(KeyEvent& event);

        static App& get() { return *s_instance; }

    protected:
        App(const ApplicationConfig& config);

        Clock* m_clock;
        f64 m_last_time;

        Window m_window;

        Allocator* m_allocator;
        Platform* m_platform;

    private:
        static App* s_instance;
    };
}

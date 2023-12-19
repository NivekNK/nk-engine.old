#pragma once

#include "nk/window.h"

namespace nk {
    class Allocator;
    class Platform;

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

        static App& get() { return *s_instance; }
        static void close() { get().m_running = false; }

    protected:
        App(const ApplicationConfig& config);

        bool m_running;
        f64 m_last_time;

        Window m_window;
        Platform* m_platform;

    private:
        static App* s_instance;
    };
}

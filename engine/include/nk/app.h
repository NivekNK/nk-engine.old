#pragma once

#include "memory/allocator.h"

namespace nk {
    class App {
    public:
        virtual ~App() = default;

        void run();

        static App& get() { return *s_instance; }
        static void close() { get().m_running = false; }

    protected:
        App(str name);

        bool m_running;
        str m_name;

    private:
        static App* s_instance;
    };
}

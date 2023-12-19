#pragma once

namespace nk {
    class Allocator;

    class SystemManager {
    public:
        ~SystemManager();

        static void init();
        static void shutdown();

        static Allocator* get_system_allocator() { return get().m_allocator; }

        static SystemManager& get() { return *s_instance; }

    private:
        SystemManager();

        Allocator* m_allocator;

        static SystemManager* s_instance;
    };
}

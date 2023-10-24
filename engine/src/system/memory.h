#pragma once

#if !defined(NK_RELEASE)

#include "memory/memory_type.h"
#include "core/hash_map.h"

namespace nk {
    class Allocator;

    template <typename T>
    class Dyarr;

    struct MemoryStats {
        str name;
        str allocator;
        MemoryType type;

        u64 size_bytes;
        u64 used_bytes;
        u64 allocation_count;
    };

    class MemorySystem {
    public:
        ~MemorySystem();

        static void init();
        static void shutdown();

        void update(const Allocator& allocator);

        void allocations(Dyarr<MemoryStats>& allocations);

        str memory_in_bytes(u64 memory);
        cstr memory_type_to_string(MemoryType memory_type);

        void log_report();

        static inline MemorySystem& get() {
            Assert(s_instance != nullptr);
            return *s_instance;
        }

    private:
        MemorySystem();

        Allocator* m_allocator;

        HashMap<cstr, MemoryStats> m_allocations;

        static MemorySystem* s_instance;
    };
}

#define MemorySystemInit()            ::nk::MemorySystem::init()
#define MemorySystemShutdown()        ::nk::MemorySystem::shutdown()
#define MemorySystemUpdate(Allocator) ::nk::MemorySystem::get().update(Allocator)
#define MemorySystemReport()          ::nk::MemorySystem::get().log_report()

#else

#define MemorySystemInit()
#define MemorySystemShutdown()
#define MemorySystemUpdate(Allocator)
#define MemorySystemReport()

#endif

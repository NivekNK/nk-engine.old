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

    class MemoryManager {
    public:
        ~MemoryManager();

        static void init();
        static void shutdown();

        void update(const Allocator& allocator);

        void allocations(Dyarr<MemoryStats>& allocations);

        str memory_in_bytes(u64 memory);
        cstr memory_type_to_string(MemoryType memory_type);

        void log_report();

        static inline MemoryManager& get() {
            Assert(s_instance != nullptr);
            return *s_instance;
        }

    private:
        MemoryManager();

        Allocator* m_allocator;
        HashMap<cstr, MemoryStats> m_allocations;

        static MemoryManager* s_instance;
    };
}

    #define MemoryManagerInit()                    ::nk::MemoryManager::init()
    #define MemoryManagerShutdown()                ::nk::MemoryManager::shutdown()
    #define MemoryManagerUpdate(Allocator)         ::nk::MemoryManager::get().update(Allocator)
    #define MemoryManagerReport()                  ::nk::MemoryManager::get().log_report()

#else

    #define MemoryManagerInit()
    #define MemoryManagerShutdown()
    #define MemoryManagerUpdate(Allocator)
    #define MemoryManagerReport()

#endif

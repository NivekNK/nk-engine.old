#pragma once

#include "core/system.h"

namespace nk {
    struct MemoryStats {
        szt allocated_bytes;
        szt total_bytes;
        u32 allocation_count;

        inline void add(szt size) {
            if (size <= 0)
                return;

            allocated_bytes += size;
            allocation_count++;
        }
    };

    class MemorySystem : public System {
    public:
        static MemorySystem* get();
        ~MemorySystem() = default;

    private:
        MemorySystem() = default;

        szt m_used_memory = 0;

        static MemorySystem s_system;
    };
}

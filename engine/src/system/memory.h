#pragma once

#include "core/system.h"
#include "core/hash_map.h"

namespace nk {
    class Allocator;

    struct MemoryStats {
        cstr name;
        cstr allocator_type;
        szt allocated_bytes;
        szt total_bytes;
        u32 allocation_count;

        inline void add(szt size) {
            if (size <= 0)
                return;

            allocated_bytes += size;
            allocation_count++;
        }

        inline void substract(szt size) {
            if (allocated_bytes - size <= 0)
                return;

            allocated_bytes -= size;
            allocation_count--;
        }
    };

    class MemorySystem : public System {
    public:
        NK_SYSTEM_INSTANCE(MemorySystem)
        ~MemorySystem();

        void insert(const cstr name, const cstr allocator_type, szt allocated_bytes, szt total_bytes, u32 allocation_count);
        bool remove(const cstr name);

        bool add(const cstr name, szt bytes);
        bool substract(const cstr name, szt bytes);

        MemoryStats* report();

    private:
        MemorySystem();

        szt m_used_memory = 0;
        Allocator* m_allocator;
        HashMap<cstr, MemoryStats> m_allocations;
    };
}

#define InsertMemory(Name, AllocatorType, AllocatedBytes, TotalBytes, AllocationCount) \
    ::nk::MemorySystem::get().insert(Name, AllocatorType, AllocatedBytes, TotalBytes, AllocationCount)

#define RemoveMemory(Name)           ::nk::MemorySystem::get().remove(Name)

#define AddMemory(Name, Bytes)       ::nk::MemorySystem::get().add(Name, Bytes)

#define SubstractMemory(Name, Bytes) ::nk::MemorySystem::get().substract(Name, Bytes)

#include "nkpch.h"

#include "system/memory.h"
#include "memory/malloc_allocator.h"

namespace nk {
    MemorySystem::MemorySystem()
        : m_allocator{new MallocAllocator("MemorySystem")},
          m_allocations{m_allocator, 16} {
    }

    MemorySystem::~MemorySystem() {
        std::free(m_allocator);
    }

    void MemorySystem::insert(const cstr name, const cstr allocator_type, szt allocated_bytes, szt total_bytes, u32 allocation_count) {
        m_allocations.insert(name, {name, allocator_type, allocated_bytes, total_bytes, allocation_count});
    }

    bool MemorySystem::remove(const cstr name) {
        return m_allocations.remove(name);
    }

    bool MemorySystem::add(const cstr name, szt bytes) {
        auto allocation = m_allocations.get(name);
        if (allocation) {
            allocation->get().add(bytes);
            return true;
        }
        return false;
    }

    bool MemorySystem::substract(const cstr name, szt bytes) {
        auto allocation = m_allocations.get(name);
        if (allocation) {
            allocation->get().substract(bytes);
            return true;
        }
        return false;
    }
}

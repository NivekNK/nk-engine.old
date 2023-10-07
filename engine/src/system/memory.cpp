#include "nkpch.h"

#include "system/memory.h"

namespace nk {
    class DebugMallocAllocator : public Allocator {
    public:
        DebugMallocAllocator(const cstr name) : Allocator(name) {}
        virtual ~DebugMallocAllocator() override = default;

        virtual void* allocate(const szt size, const szt alignment) override {
            return std::malloc(size);
        }

        virtual void free(void* ptr) override {
            std::free(ptr);
        }
    };

    MemorySystem::MemorySystem()
        : m_allocator{new DebugMallocAllocator("MemorySystem")},
          m_allocations{m_allocator, 64} {
    }

    MemorySystem::~MemorySystem() {
        std::free(m_allocator);
    }

    void MemorySystem::insert(cstr name, const cstr allocator_type, szt allocated_bytes, szt total_bytes, u32 allocation_count) {
        m_allocations.insert(name, {name, allocator_type, allocated_bytes, total_bytes, allocation_count});
    }

    bool MemorySystem::remove(cstr name) {
        return m_allocations.remove(name);
    }

    bool MemorySystem::add(cstr name, szt bytes) {
        auto allocation = m_allocations.get(name);
        if (allocation) {
            allocation->get().add(bytes);
            return true;
        }
        return false;
    }

    bool MemorySystem::substract(cstr name, szt bytes) {
        auto allocation = m_allocations.get(name);
        if (allocation) {
            allocation->get().substract(bytes);
            return true;
        }
        return false;
    }

    MemoryStats* MemorySystem::report() {
        return nullptr;
    }

    void MemorySystem::debug_info_report() {}
}

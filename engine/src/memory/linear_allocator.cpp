#include "nkpch.h"

#include "memory/linear_allocator.h"

namespace nk {
    LinearAllocator::LinearAllocator(const cstr name, const szt size)
        : Allocator(name),
          m_memory{static_cast<u8*>(std::malloc(size))},
          m_total_size{size},
          m_allocated_size{0} {}

    LinearAllocator::~LinearAllocator() {
        clear();
        std::free(m_memory);
    }

    void* LinearAllocator::allocate(const szt size, const szt alignment) {
        Assert(size > 0);

        const szt new_start = memory_align(m_allocated_size, alignment);
        Assert(new_start < m_total_size);
        const szt new_allocated_size = new_start + size;
        AssertMsg(new_allocated_size <= m_total_size, "Overflow.");

        m_allocated_size = new_allocated_size;
        return m_memory + new_start;
    }

    void LinearAllocator::free(void* ptr) {
        AssertMsg(false, "LinearAllocator::free() should not be called.");
    }

    void LinearAllocator::clear() {
        m_allocated_size = 0;
    }
}

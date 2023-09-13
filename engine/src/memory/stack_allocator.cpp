#include "nkpch.h"

#include "memory/stack_allocator.h"

namespace nk {
    StackAllocator::StackAllocator(const cstr name, const szt size)
        : Allocator(name),
          m_memory{static_cast<u8*>(std::malloc(size))},
          m_allocated_size{0},
          m_total_size{size} {
    }

    StackAllocator::~StackAllocator() {
        std::free(m_memory);
    }

    void* StackAllocator::allocate(const szt size, const szt alignment) {
        Assert(size > 0);

        const szt new_start = memory_align(m_allocated_size, alignment);
        Assert(new_start < m_total_size);

        const szt new_allocated_size = new_start + size;
        AssertMsg(new_allocated_size <= m_total_size, "Overflow.");

        m_allocated_size = new_allocated_size;
        return m_memory + new_start;
    }

    void StackAllocator::free(void* ptr) {
        Assert(ptr >= m_memory);

        const szt size_at_ptr = static_cast<u8*>(ptr) - m_memory;
        AssertMsg(ptr < m_memory + m_total_size,
                  "Out of bound free on StackAllocator (outside of bounds). Tempting to free {:p}, {} after beginning of buffer (memory: {:p}, size: {}, allocated: {}).",
                  ptr, size_at_ptr, static_cast<void*>(m_memory), m_total_size, m_allocated_size);
        AssertMsg(ptr < m_memory + m_allocated_size,
                  "Out of bound free on StackAllocator (inside bounds, after allocated). Tempting to free {:p}, {} after beginning of buffer (memory: {:p}, size: {}, allocated: {}).",
                  ptr, size_at_ptr, static_cast<void*>(m_memory), m_total_size, m_allocated_size);

        m_allocated_size = size_at_ptr;
    }

    szt StackAllocator::get_marker() {
        return m_allocated_size;
    }

    void StackAllocator::free_marker(const szt marker) {
        const szt difference = marker - m_allocated_size;
        if (difference <= 0)
            return;
        m_allocated_size = marker;
    }

    void StackAllocator::clear() {
        m_allocated_size = 0;
    }
}

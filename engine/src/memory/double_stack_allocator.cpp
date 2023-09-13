#include "nkpch.h"

#include "memory/double_stack_allocator.h"

namespace nk {
    DoubleStackAllocator::DoubleStackAllocator(const cstr name, const szt size)
        : Allocator(name),
          m_memory{static_cast<u8*>(std::malloc(size))},
          m_top{size},
          m_bottom{0},
          m_total_size{size} {}

    DoubleStackAllocator::~DoubleStackAllocator() {
        std::free(m_memory);
    }

    void* DoubleStackAllocator::allocate(const szt size, const szt alignment) {
        AssertMsg(false, "DoubleStackAllocator::allocate() should not be called.");
        return nullptr;
    }

    void DoubleStackAllocator::free(void* ptr) {
        AssertMsg(false, "DoubleStackAllocator::free() should not be called.");
    }

    void* DoubleStackAllocator::allocate_top(const szt size, const szt alignment) {
        Assert(size > 0);

        const szt new_start = memory_align(m_bottom, alignment);
        AssertMsg(new_start > m_bottom, "Overflow Crossing.");

        m_top = new_start;
        return m_memory + new_start;
    }

    void DoubleStackAllocator::free_top(const szt size) {
        if (size > m_total_size - m_top) {
            m_top = m_total_size;
        } else {
            m_top += size;
        }
    }

    void* DoubleStackAllocator::allocate_bottom(const szt size, const szt alignment) {
        Assert(size > 0);

        const szt new_start = memory_align(m_bottom, alignment);
        const szt new_allocated_size = new_start + size;
        AssertMsg(new_allocated_size < m_top, "Overflow Crossing.");

        m_bottom = new_allocated_size;
        return m_memory + new_start;
    }

    void DoubleStackAllocator::free_bottom(const szt size) {
        if (size > m_bottom) {
            m_bottom = 0;
        } else {
            m_bottom -= size;
        }
    }

    szt DoubleStackAllocator::get_top_marker() {
        return m_top;
    }

    void DoubleStackAllocator::free_top_marker(const szt marker) {
        if (marker > m_top && marker < m_total_size)
            m_top = marker;
    }

    szt DoubleStackAllocator::get_bottom_marker() {
        return m_bottom;
    }

    void DoubleStackAllocator::free_bottom_marker(const szt marker) {
        if (marker < m_bottom)
            m_bottom = marker;
    }

    void DoubleStackAllocator::clear_top() {
        m_top = m_total_size;
    }

    void DoubleStackAllocator::clear_bottom() {
        m_bottom = 0;
    }
}

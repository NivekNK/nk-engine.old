#include "nkpch.h"

#include "memory/allocator.h"

#include "system/memory.h"

namespace nk {
    Allocator::Allocator(const u64 size_bytes, void* const start, str name, MemoryType type)
        : m_size_bytes{size_bytes},
          m_used_bytes{0},
          m_allocation_count{0},
#if !defined(NK_RELEASE)
          m_start{start},
          m_name{name},
          m_type{type}
#else
          m_start{start}
#endif
    {
    }

    Allocator::Allocator(Allocator&& other)
        : m_size_bytes{other.m_size_bytes},
          m_used_bytes{other.m_used_bytes},
          m_allocation_count{other.m_allocation_count},
#if !defined(NK_RELEASE)
          m_start{other.m_start},
          m_name{other.m_name},
          m_type{other.m_type}
#else
          m_start{other.m_start}
#endif
    {
        other.m_size_bytes = 0;
        other.m_used_bytes = 0;
        other.m_allocation_count = 0;
        other.m_start = nullptr;
#if !defined(NK_RELEASE)
        other.m_name = "Invalid";
        other.m_type = MemoryType::None;
#endif
    }

    Allocator& Allocator::operator=(Allocator&& rhs) {
        m_size_bytes = rhs.m_size_bytes;
        m_used_bytes = rhs.m_used_bytes;
        m_allocation_count = rhs.m_allocation_count;
        m_start = rhs.m_start;
#if !defined(NK_RELEASE)
        m_name = rhs.m_name;
        m_type = rhs.m_type;
#endif

        rhs.m_size_bytes = 0;
        rhs.m_used_bytes = 0;
        rhs.m_allocation_count = 0;
        rhs.m_start = nullptr;
#if !defined(NK_RELEASE)
        rhs.m_name = "Invalid";
        rhs.m_type = MemoryType::None;
#endif

        return *this;
    }

    Allocator::~Allocator() {
        Assert(m_allocation_count == 0 && m_used_bytes == 0);
    }
}

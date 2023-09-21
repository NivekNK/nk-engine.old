#include "nkpch.h"

#include "memory/heap_allocator.h"
#include "system/memory.h"

#include "tlsf/tlsf.h"

#define NK_HEAP_ALLOCATOR_STATS 1

namespace nk {
    void exit_walker(void* ptr, const szt size, const i32 used, void* user) {
        MemoryStats* stats = static_cast<MemoryStats*>(user);
        stats->add(used ? size : 0);
        DebugIfLog(used, "Found active allocation {}, {}", ptr, size);
    }

    HeapAllocator::HeapAllocator(const cstr name, const szt size)
        : Allocator(name),
          m_memory{std::malloc(size)},
          m_max_size{size},
          m_allocated_size{0} {
        m_tlsf_handle = tlsf_create_with_pool(m_memory, size);
        DebugLog("HeapAllocator of size {} created", size);
    }

    HeapAllocator::~HeapAllocator() {
        MemoryStats stats{m_name, "HeapAllocator", 0, m_max_size};
        pool_t pool = tlsf_get_pool(m_tlsf_handle);
        tlsf_walk_pool(pool, exit_walker, static_cast<void*>(&stats));

        AssertMsg(stats.allocated_bytes == 0, "Allocations still present, memory leak!");

        tlsf_destroy(m_tlsf_handle);
        std::free(m_memory);
    }

    void* HeapAllocator::allocate(const szt size, [[maybe_unused]] const szt alignment) {
        void* memory;
#if NK_HEAP_ALLOCATOR_STATS
        memory = alignment == 1 ? tlsf_malloc(m_tlsf_handle, size)
                                : tlsf_memalign(m_tlsf_handle, alignment, size);
        szt actual_size = tlsf_block_size(memory);
        m_allocated_size += actual_size;

#else
        memory = tlsf_malloc(m_tlsf_handle, size);
#endif
        return memory;
    }

    void HeapAllocator::free(void* ptr) {
#if NK_HEAP_ALLOCATOR_STATS
        szt actual_size = tlsf_block_size(ptr);
        m_allocated_size -= actual_size;

        tlsf_free(m_tlsf_handle, ptr);
#else
        tlsf_free(m_tlsf_handle, ptr);
#endif
    }
}

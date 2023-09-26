#include "nkpch.h"

#include "memory/malloc_allocator.h"

#if !defined(NK_RELEASE)
    #include "system/memory.h"
#endif

namespace nk {
    void* MallocAllocator::allocate(const szt size, const szt alignment) {
        DebugLog("Trying to allocate");
        // InsertMemory(m_name, "MallocAllocator", size, size, 1);
        // DebugLog("Inserted to map");
        return std::malloc(size);
    }

    void MallocAllocator::free(void* ptr) {
        // RemoveMemory(m_name);
        std::free(ptr);
    }
}

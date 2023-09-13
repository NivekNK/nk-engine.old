#include "nkpch.h"

#include "memory/malloc_allocator.h"

namespace nk {
    void* MallocAllocator::allocate(const szt size, const szt alignment) {
        return std::malloc(size);
    }

    void MallocAllocator::free(void* ptr) {
        std::free(ptr);
    }
}

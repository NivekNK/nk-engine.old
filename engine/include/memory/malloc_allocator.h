#pragma once

#include "memory/allocator.h"

namespace nk {
    class MallocAllocator : public Allocator {
    public:
        MallocAllocator([[maybe_unused]] str name, [[maybe_unused]] MemoryType type);

        MallocAllocator(const MallocAllocator&) = delete;
        MallocAllocator& operator=(MallocAllocator&) = delete;

        MallocAllocator(MallocAllocator&&);
        MallocAllocator& operator=(MallocAllocator&&);

        virtual ~MallocAllocator() override;

#if !defined(NK_RELEASE)
        virtual const cstr to_string() const override {
            return "MallocAllocator";
        }
#endif

        virtual void* allocate(const u64 size_bytes, [[maybe_unused]] const u64 alignment) override;
        virtual void free(void* const ptr, [[maybe_unused]] const u64 size_bytes) override;
    };
}

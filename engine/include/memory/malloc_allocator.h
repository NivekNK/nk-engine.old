#pragma once

#include "memory/allocator.h"

namespace nk {
    class MallocAllocator : public Allocator {
    public:
        MallocAllocator(const cstr name) : Allocator(name) {}
        virtual ~MallocAllocator() override = default;

        virtual void* allocate(const szt size, const szt alignment) override;
        virtual void free(void* ptr) override;
    };
}

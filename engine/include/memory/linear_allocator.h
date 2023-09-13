#pragma once

#include "memory/allocator.h"

namespace nk {
    class LinearAllocator : public Allocator {
    public:
        LinearAllocator(const cstr name, const szt size);
        virtual ~LinearAllocator() override;

        virtual void* allocate(const szt size, const szt alignment) override;
        virtual void free(void* ptr) override;

        void clear();

    private:
        u8* m_memory = nullptr;
        szt m_total_size = 0;
        szt m_allocated_size = 0;
    };
}

#pragma once

#include "memory/allocator.h"

namespace nk {
    class HeapAllocator : public Allocator {
    public:
        HeapAllocator(const cstr name, const szt size);
        virtual ~HeapAllocator() override;

        virtual void* allocate(const szt size, const szt alignment) override;
        virtual void free(void* ptr) override;

    private:
        void* m_tlsf_handle;
        void* m_memory;
        szt m_allocated_size = 0;
        szt m_max_size = 0;
    };
}

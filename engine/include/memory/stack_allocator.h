#pragma once

#include "memory/allocator.h"

namespace nk {
    class StackAllocator : public Allocator {
    public:
        StackAllocator(const cstr name, szt size);
        virtual ~StackAllocator() override;

        virtual void* allocate(const szt size, const szt alignment) override;
        virtual void free(void* ptr) override;

        szt get_marker();
        void free_marker(const szt marker);

        void clear();

    private:
        u8* m_memory = nullptr;
        szt m_total_size = 0;
        szt m_allocated_size = 0;
    };
}

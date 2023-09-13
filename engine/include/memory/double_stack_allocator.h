#pragma once

#include "memory/allocator.h"

namespace nk {
    class DoubleStackAllocator : public Allocator {
    public:
        DoubleStackAllocator(const cstr name, const szt size);
        virtual ~DoubleStackAllocator() override;

        virtual void* allocate(const szt size, const szt alignment) override;
        virtual void free(void* ptr) override;

        void* allocate_top(const szt size, const szt alignment);
        void free_top(const szt size);

        void* allocate_bottom(const szt size, const szt alignment);
        void free_bottom(const szt size);

        szt get_top_marker();
        void free_top_marker(const szt marker);

        szt get_bottom_marker();
        void free_bottom_marker(const szt marker);

        void clear_top();
        void clear_bottom();

    private:
        u8* m_memory = nullptr;
        szt m_total_size = 0;
        szt m_top = 0;
        szt m_bottom = 0;
    };
}

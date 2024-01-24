#pragma once

#include "memory/allocator.h"
#include "core/dyarr.h"

namespace nk {
    struct DeferedMemory {
        void* memory;
        u64 size;
    };

    template <typename T>
    concept IAllocator = std::is_same_v<T, Allocator>;

    template <IAllocator A>
    class Defer {
    public:
        template <typename ...Args>
        Defer(Args&&... args) {
            m_allocator = new A(std::forward<Args>(args)...);
            m_memory.init(m_allocator, 15);
        }

        ~Defer() {
            for (DeferedMemory& defered : m_memory) {
                m_allocator->free(defered.memory, defered.size);
            }
            m_memory.clear();
        }

        template <typename T>
        inline T* allocate() {
            const u64 size = sizeof(T);
            void* memory = allocate(size, alignof(T));
            m_memory.push({
                .memory = memory,
                .size = size,
            });
            return static_cast<T*>(memory);
        }

        template <typename T>
        inline T* allocate(const u64 lot) {
            const u64 size = sizeof(T) * lot;
            void* memory = allocate(size, alignof(T));
            m_memory.push({
                .memory = memory,
                .size = size,
            });
            return static_cast<T*>(memory);
        }

        void* allocate(const u64 size_bytes, const u64 alignment) {
            return m_allocator->allocate(size_bytes, alignment);
        }

    private:
        Allocator* m_allocator;
        Dyarr<DeferedMemory> m_memory;
    };
}

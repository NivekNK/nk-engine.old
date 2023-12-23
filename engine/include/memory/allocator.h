#pragma once

#include "memory/memory_type.h"

namespace nk {
    class Allocator {
    public:
        Allocator(const u64 size_bytes, void* const start, [[maybe_unused]] str name, [[maybe_unused]] MemoryType type);

        Allocator(const Allocator&) = delete;
        Allocator& operator=(Allocator&) = delete;

        Allocator(Allocator&&);
        Allocator& operator=(Allocator&&);

        virtual ~Allocator();

        template <typename T>
        constexpr inline T* allocate(const u64 lot = 1) {
            return static_cast<T*>(allocate(sizeof(T) * lot, alignof(T)));
        }

        template <typename T, typename... Args>
        inline T* construct(Args&&... args) {
            return new (allocate(sizeof(T), alignof(T))) T(std::forward<Args>(args)...);
        }

        template <typename T, typename V>
        inline void destroy(V* ptr) {
            if (ptr == nullptr) {
                WarnLog("Trying to destroy nullptr!");
                return;
            }

            ptr->~V();
            free(ptr, sizeof(T));
        }

        inline u64 size() const { return m_size_bytes; }
        inline u64 used() const { return m_used_bytes; }
        inline u64 allocation_count() const { return m_allocation_count; }
        inline const void* start() const { return m_start; }

#if !defined(NK_RELEASE)
        inline cstr name() const { return m_name.c_str(); }
        inline MemoryType type() const { return m_type; }

        virtual const cstr to_string() const = 0;
#endif

        virtual void* allocate(const u64 size_bytes, [[maybe_unused]] const u64 alignment) = 0;
        virtual void free(void* const ptr, [[maybe_unused]] const u64 size_bytes) = 0;

    protected:
        u64 m_size_bytes;
        u64 m_used_bytes;
        u64 m_allocation_count;

        void* m_start;

#if !defined(NK_RELEASE)
        str m_name;
        MemoryType m_type;
#endif
    };
}

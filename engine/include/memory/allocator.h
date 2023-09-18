#pragma once

namespace nk {
    class Allocator {
    public:
        Allocator([[maybe_unused]] const cstr name)
#if !defined(NK_RELEASE)
            : m_name{name}
#endif
        {
        }

        virtual ~Allocator() = default;

        Allocator(const Allocator&) = delete;
        Allocator& operator=(Allocator&) = delete;

        virtual void* allocate(const szt size, const szt alignment) = 0;
        virtual void free(void* ptr) = 0;

        inline void* allocate(const szt size) { return allocate(size, 1); }
        inline u8* allocatem(const szt size) { return static_cast<u8*>(allocate(size, 1)); }

        template <typename T>
        inline T* allocate() { return static_cast<T*>(allocate(sizeof(T), 1)); }

        template <typename T>
        inline T* allocate(const szt size) { return static_cast<T*>(allocate(size, 1)); }

    protected:
        inline szt memory_align(const szt size, const szt alignment) {
            const szt alignment_mask = alignment - 1;
            return (size + alignment_mask) & ~alignment_mask;
        }

#if !defined(NK_RELEASE)
        const cstr m_name;
#endif
    };
}

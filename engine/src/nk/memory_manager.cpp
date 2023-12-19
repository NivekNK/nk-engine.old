#include "nkpch.h"

#include "core/dyarr.h"
#include "nk/memory_manager.h"

#include "memory/allocator.h"

namespace nk {
    class DebugMallocAllocator : public Allocator {
    public:
        DebugMallocAllocator() : Allocator(0, nullptr, "DebugMalloc", MemoryType::None) {}
        virtual ~DebugMallocAllocator() override = default;

        virtual void* allocate(const szt size, const szt alignment) override {
            return std::malloc(size);
        }

        virtual void free(void* const ptr, const u64 size_bytes) override {
            std::free(ptr);
        }

        virtual const cstr to_string() const override {
            return "DebugMallocAllocator";
        }
    };

    MemoryManager* MemoryManager::s_instance = nullptr;

    MemoryManager::MemoryManager()
        : m_allocator{new DebugMallocAllocator()},
          m_allocations{m_allocator, 32} {
    }

    MemoryManager::~MemoryManager() {
        std::free(m_allocator);
    }

    void MemoryManager::init() {
        s_instance = new MemoryManager();
    }

    void MemoryManager::shutdown() {
        delete s_instance;
    }

    void MemoryManager::update(const Allocator& allocator) {
        m_allocations.insert(allocator.name(), {
            .name = allocator.name(),
            .allocator = allocator.to_string(),
            .type = allocator.type(),
            .size_bytes = allocator.size(),
            .used_bytes = allocator.used(),
            .allocation_count = allocator.allocation_count(),
        });
    }

    void MemoryManager::allocations(Dyarr<MemoryStats>& stats) {
        stats.set_capacity(m_allocations.size());
        stats.set_length(m_allocations.size());

        u32 index = 0;
        for (auto it = m_allocations.it_begin(); it.is_valid(); m_allocations.it_advance(it)) {
            auto value = m_allocations.get(it);
            if (value) {
                stats[index] = value->get();
                index++;
            }
        }
    }

    str MemoryManager::memory_in_bytes(u64 memory) {
        // TODO: Add to math library
        static constexpr u64 gib = 1024 * 1024 * 1024;
        static constexpr u64 mib = 1024 * 1024;
        static constexpr u64 kib = 1024;

        if (memory >= gib) {
            return std::format("{:.2f} GiB", memory / static_cast<f32>(gib));
        } else if (memory >= mib) {
            return std::format("{:.2f} MiB", memory / static_cast<f32>(mib));
        } else if (memory >= kib) {
            return std::format("{:.2f} KiB", memory / static_cast<f32>(kib));
        } else {
            return std::format("{:.2f} B", static_cast<f32>(memory));
        }
    }

    cstr MemoryManager::memory_type_to_string(MemoryType memory_type) {
        switch (memory_type) {
            case MemoryType::None:
                return "None (Invalid)";
            case MemoryType::Editor:
                return "Editor";
            case MemoryType::Application:
                return "Application";
            case MemoryType::MaxTypes:
                return "Invalid";
            default:
                return "Not Implemented";
        }
    }

    void MemoryManager::log_report() {
        u64 total_allocated = 0;

        u64 type_allocated[static_cast<u32>(MemoryType::MaxTypes)] = {0};
        u64 type_used[static_cast<u32>(MemoryType::MaxTypes)] = {0};

        StyledLog("\n\nDetailed Memory Usage:\n", 4, -1, 1);
        for (auto it = m_allocations.it_begin(); it.is_valid(); m_allocations.it_advance(it)) {
            auto value = m_allocations.get(it);
            if (value) {
                type_allocated[static_cast<u32>(value->get().type)] += value->get().size_bytes;
                type_used[static_cast<u32>(value->get().type)] += value->get().used_bytes;

                total_allocated += value->get().size_bytes;

                str stats = std::format(
                    "{}:\n    - Allocator: {}\n    - Type: {}\n    - Allocated: {}\n    - Used: {}\n    - Allocation Count: {}\n",
                    value->get().name, value->get().allocator, memory_type_to_string(value->get().type), memory_in_bytes(value->get().size_bytes), memory_in_bytes(value->get().used_bytes), value->get().allocation_count);
                StyledLog(stats.c_str(), 7, -1, -1);
            }
        }

        StyledLog("\nGeneral Memory Usage:\n", 4, -1, 1);
        for (u32 i = 0; i < static_cast<u32>(MemoryType::MaxTypes); i++) {
            str stats = std::format(
                "{}:\n    - Allocated: {}\n    - Used: {}\n",
                memory_type_to_string(static_cast<MemoryType>(i)), memory_in_bytes(type_allocated[i]), memory_in_bytes(type_used[i]));
            StyledLog(stats.c_str(), 7, -1, -1);
        }

        str stats = std::format("\nTOTAL ALLOCATED MEMORY: {}\n\n", memory_in_bytes(total_allocated));
        StyledLog(stats.c_str(), 4, -1, 1);
    }
}

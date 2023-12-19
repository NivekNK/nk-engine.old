#include "nkpch.h"

#include "nk/system_manager.h"

#include "memory/malloc_allocator.h"

namespace nk {
    SystemManager* SystemManager::s_instance = nullptr;

    SystemManager::SystemManager() {
        m_allocator = new MallocAllocator("System", MemoryType::Application);
    }

    SystemManager::~SystemManager() {
        delete m_allocator;
    }

    void SystemManager::init() {
        s_instance = new SystemManager();
    }

    void SystemManager::shutdown() {
        delete s_instance;
    }
}

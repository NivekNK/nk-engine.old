#include "nkpch.h"

#include "nk/app_creator.h"

#include "nk/memory_manager.h"
#include "memory/malloc_allocator.h"

#include "system/input.h"

namespace nk {
    i32 entry_point(i32 argc, char** argv) {
        MemoryManagerInit();

        {
            MallocAllocator allocator{"System", MemoryType::Application};

            LoggingSystem::init();
            InputSystem::init();

            auto app = create_application(&allocator);
            MemoryManagerReport();
            app->run();
            destroy_application(&allocator, app);

            InputSystem::shutdown();
            LoggingSystem::shutdown();
        }

        MemoryManagerReport();
        MemoryManagerShutdown();

        return EXIT_SUCCESS;
    }
}

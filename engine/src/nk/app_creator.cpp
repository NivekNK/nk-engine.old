#include "nkpch.h"

#include "nk/app_creator.h"

#include "nk/memory_manager.h"
#include "nk/system_manager.h"
#include "memory/allocator.h"

namespace nk {
    i32 entry_point(i32 argc, char** argv) {
        MemoryManagerInit();

        {
            SystemManager::init();

            auto allocator = SystemManager::get_system_allocator();
            auto app = create_application(allocator);
            MemoryManagerReport();
            app->run();
            allocator->destroy(app);

            SystemManager::shutdown();
        }

        MemoryManagerReport();
        MemoryManagerShutdown();

        return EXIT_SUCCESS;
    }
}

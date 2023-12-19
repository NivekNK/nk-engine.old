#include "nkpch.h"

#include "nk/app_creator.h"

#include "system/memory.h"
#include "memory/malloc_allocator.h"

namespace nk {
    i32 entry_point(i32 argc, char** argv) {
        MemorySystemInit();

        {
            MallocAllocator allocator{"App", MemoryType::Application};
            auto app = create_application(&allocator);
            MemorySystemReport();
            app->run();
            allocator.destroy(app);
        }

        MemorySystemReport();
        MemorySystemShutdown();

        return EXIT_SUCCESS;
    }
}

#include "memory/malloc_allocator.h"
#include "test.h"

int main(void) {
    init();

    {
        nk::MallocAllocator allocator{"Test", nk::MemoryType::Editor};

        void* test = allocator.allocate(KiB(1), 1);

        void* another_test = allocator.allocate(16, 1);

        test_function();

        allocator.free(test, KiB(1));

        allocator.free(another_test, 16);

        test_function();
    }

    shutdown();

    return 0;
}

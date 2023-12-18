#include "memory/malloc_allocator.h"
#include "test.h"

#include "core/file.h"

int main(void) {
    init();

    {
        nk::MallocAllocator allocator{"TestFile", nk::MemoryType::Editor};
        nk::File* file = nk::File::open(&allocator, "./test.txt", nk::FileMode::Read | nk::FileMode::Write);


        test_function();

        nk::str line;
        while (file->read_line_string(line, 512)) {
            std::cout << line << std::endl;
        }

        char text[] = "Hello world!";
        file->write(text, 12, true, false);

        file->close();
        allocator.destroy(file);
    }

    test_function();

    shutdown();

    return 0;
}

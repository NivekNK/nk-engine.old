#include "test.h"
#include "system/memory.h"

void init() {
    MemorySystemInit();
}

void test_function() {
    MemorySystemReport();
}

void shutdown() {
    MemorySystemShutdown();
}

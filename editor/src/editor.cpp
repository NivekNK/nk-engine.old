#include "core/hash_map.h"
#include "memory/malloc_allocator.h"

int main(void) {
    nk::MallocAllocator allocator{"test"};
    nk::HashMap<nk::cstr, nk::u64> test{&allocator, 16};
    DebugLog("Finished HashMap Creation");

    test.insert("test1", 1);
    test.insert("test2", 2);
    test.insert("test3", 3);
    test.insert("test4", 4);

    auto test4 = test.get("test4");
    DebugIfLog(test4, "Test4: {}", test4->get());
    auto test1 = test.get("test1");
    DebugIfLog(test1, "Test1: {}", test1->get());
    auto test3 = test.get("test3");
    DebugIfLog(test3, "Test3: {}", test1->get());
    auto test2 = test.get("test2");
    DebugIfLog(test2, "Test2: {}", test3->get());
    auto test5 = test.get("test5");
    DebugIfLog(test5, "Test5: {}", test5->get());

    return 0;
}

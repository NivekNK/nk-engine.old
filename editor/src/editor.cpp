#include "core/arr.h"
#include "core/dyarr.h"
#include "memory/malloc_allocator.h"

void check_array(nk::Arr<nk::u32> check) {
    for (nk::u32 i = 0; i < check.length(); i++) {
        DebugLog("Check > Index: {} | Value: {}", i, check[i]);
    }
}

int main(void) {
    nk::MallocAllocator allocator{"test"};
    nk::Dyarr<nk::u32> test{&allocator, 12};
    test.push(1213);
    test.push(223);
    test.push(213);

    auto& value = test.push_use();
    value = 45;

    test.insert(2000, 1);
    auto& insert_value = test.insert_use(10);
    insert_value = 8000000;

    for (nk::u32 i = 0; i < test.length(); i++) {
        DebugLog("Index: {} | Value: {}", i, test[i]->get());
    }

    InfoLog("---------------");

    test.pop();
    test.pop();
    test.remove_swap_tail(1);
    test.remove(3);

    for (nk::u32 i = 0; i < test.length(); i++) {
        DebugLog("Index: {} | Value: {}", i, test[i]->get());
    }

    check_array(test);

    return 0;
}

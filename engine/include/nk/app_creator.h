#pragma once

#include "nk/app.h"
#include "memory/allocator.h"

namespace nk {
    App* create_application(Allocator* allocator);

    i32 entry_point(i32 argc, char** argv);
}

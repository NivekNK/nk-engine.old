#pragma once

#include "nk/app_creator.h"

int main(int argc, char** argv) {
    return nk::entry_point(argc, argv);
}

#define CREATE_APP(AppType, ...)                                                   \
    nk::App* nk::create_application(nk::Allocator* allocator) {                    \
        auto app = allocator->construct<AppType>(__VA_ARGS__);                     \
        return app;                                                                \
    }                                                                              \
    void nk::destroy_application(nk::Allocator* allocator, nk::App* application) { \
        allocator->destroy<AppType>(application);                                  \
    }

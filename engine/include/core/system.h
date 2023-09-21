#pragma once

namespace nk {
    class System {
    protected:
        System() = default;
    };
}

#define NK_SYSTEM_INSTANCE(Type) \
    static Type& get() {         \
        static Type instance;    \
        return instance;         \
    }

#pragma once

namespace nk {
    enum class MemoryType {
        None = 0,

        Editor,
        Application,
        Renderer,
        Defer,

        MaxTypes,
    };
}

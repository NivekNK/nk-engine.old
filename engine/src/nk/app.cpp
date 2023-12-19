#include "nkpch.h"

#include "nk/app.h"

// TEST: Remove
#include "platform/platform_win32.h"

namespace nk {
    App* App::s_instance = nullptr;

    App::App(str name)
        : m_name{name},
          m_running{true} {
        Assert(s_instance == nullptr);
        s_instance = this;
    }

    void App::run() {
        PlatformWin32 platform{"NK Engine", 100, 100, 1280, 720};
        while(m_running) {
            platform.pump_messages();
        }
    }
}

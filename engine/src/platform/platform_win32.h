#pragma once

#include "platform/platform.h"

#if defined(NK_PLATFORM_WINDOWS)
namespace nk {
    class PlatformWin32 : public Platform {
    public:
        PlatformWin32(str application_name, const u32 x, const u32 y, const u32 width, const u32 height);
        virtual ~PlatformWin32() override;

        virtual bool pump_messages() override;
        virtual f64 get_absolute_time() override;
        virtual void sleep(u64 ms) override;

        virtual void get_required_extensions(Dyarr<cstr>& extensions) const override;
        virtual VkSurfaceKHR create_surface(Instance& instance, VkAllocationCallbacks* allocator) const override;;

    private:
        HINSTANCE m_hinstance;
        HWND m_hwnd;

        static f64 s_clock_frequency;
        static LARGE_INTEGER s_start_time;
    };
}
#endif

#pragma once

namespace nk {
    class Platform {
    public:
        virtual ~Platform() = default;

        virtual bool pump_messages() = 0;
        virtual f64 get_absolute_time() = 0;
        virtual void sleep(u64 ms) = 0;

    protected:
        Platform(str application_name, const u32 x, const u32 y, const u32 width, const u32 height) {}
    };
}

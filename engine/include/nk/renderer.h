#pragma once

namespace nk {
    struct RenderPacket {
        f64 delta_time;
    };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual bool draw_frame(const RenderPacket& packet) = 0;
    };
}

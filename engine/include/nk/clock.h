#pragma once

namespace nk {
    class Platform;

    class Clock {
    public:
        Clock(Platform& platform);

        ~Clock() = default;

        void start();
        void update();
        void stop();

        f64 elapsed() const { return m_elapsed; }

    private:
        Platform& m_platform;
        f64 m_start_time = 0;
        f64 m_elapsed = 0;
    };
}

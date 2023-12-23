#include "nkpch.h"

#include "nk/clock.h"

#include "platform/platform.h"

namespace nk {
    Clock::Clock(Platform& platform)
        : m_platform{platform} {
    }

    void Clock::start() {
        m_start_time = m_platform.get_absolute_time();
        m_elapsed = 0;
    }

    void Clock::update() {
        if (m_start_time == 0)
            return;

        m_elapsed = m_platform.get_absolute_time() - m_start_time;
    }

    void Clock::stop() {
        m_start_time = 0;
    }
}

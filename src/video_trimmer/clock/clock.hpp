#pragma once

#include <chrono>

#include "duration.hpp"

namespace video_trimmer::clock {

class Clock {
    std::chrono::steady_clock::time_point start_;

public:
    Clock() : start_{std::chrono::steady_clock::now()} { }

    Duration elapsed_time() const
    {
        return Duration{std::chrono::steady_clock::now() - start_};
    }

    Duration restart()
    {
        const auto now = std::chrono::steady_clock::now();
        const auto dur = now - start_;
        start_ = now;
        return Duration{dur};
    }
};

}  // namespace video_trimmer::clock

#pragma once

#include <chrono>

#include "duration.hpp"

namespace video_trimmer::clock {

class Clock {
    std::chrono::steady_clock::time_point start_;

public:
    Clock() : start_{std::chrono::steady_clock::now()} { }

    Duration elapsed_time()
    {
        auto now = std::chrono::steady_clock::now();
        return Duration{now - start_};
    }

    Duration restart()
    {
        auto now = std::chrono::steady_clock::now();
        auto dur = now - start_;
        start_ = now;
        return Duration{dur};
    }
};

}  // namespace video_trimmer::clock

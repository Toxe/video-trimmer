#pragma once

#include <chrono>

#include "playback_time_point.hpp"

namespace video_trimmer::clock {

class PlaybackClock {
public:
    PlaybackClock() : start_{std::chrono::steady_clock::now()} { }

    [[nodiscard]] PlaybackTimePoint now() const { return PlaybackTimePoint{std::chrono::steady_clock::now() - start_.microseconds()}; }

private:
    PlaybackTimePoint start_;
};

}  // namespace video_trimmer::clock

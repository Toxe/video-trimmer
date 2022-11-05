#pragma once

#include <chrono>

#include "playback_position.hpp"

namespace video_trimmer::clock {

class PlaybackTimePoint {
public:
    PlaybackTimePoint() : tp_{std::chrono::microseconds{0}} { }
    explicit PlaybackTimePoint(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> tp) : tp_{tp} { }
    explicit PlaybackTimePoint(const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> tp) : tp_{std::chrono::time_point_cast<std::chrono::microseconds>(tp)} { }

    PlaybackTimePoint operator+(const std::chrono::microseconds& us) const { return PlaybackTimePoint{tp_ + us}; }
    PlaybackTimePoint operator-(const std::chrono::microseconds& us) const { return PlaybackTimePoint{tp_ - us}; }
    PlaybackPosition operator-(const PlaybackTimePoint& other) const { return PlaybackPosition{microseconds() - other.microseconds()}; }

    [[nodiscard]] std::chrono::microseconds microseconds() const { return tp_.time_since_epoch(); }

private:
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> tp_;
};

}  // namespace video_trimmer::clock

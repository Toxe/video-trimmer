#pragma once

#include "microsecond_duration.hpp"

namespace video_trimmer::clock {

class PlaybackPosition : public MicrosecondDuration {
public:
    PlaybackPosition() = default;
    explicit PlaybackPosition(std::chrono::microseconds us) : MicrosecondDuration{us} { }
    explicit PlaybackPosition(const MicrosecondDuration& dur) : MicrosecondDuration{dur.microseconds()} { }

    PlaybackPosition& operator=(const std::chrono::microseconds& us) { return *this = PlaybackPosition{us}; }

    PlaybackPosition operator+(const MicrosecondDuration& dur) const { return PlaybackPosition{microseconds() + dur.microseconds()}; }
    PlaybackPosition operator-(const MicrosecondDuration& dur) const { return PlaybackPosition{microseconds() - dur.microseconds()}; }
    PlaybackPosition operator+(const std::chrono::microseconds& us) const { return PlaybackPosition{microseconds() + us}; }
    PlaybackPosition operator-(const std::chrono::microseconds& us) const { return PlaybackPosition{microseconds() - us}; }
};

}  // namespace video_trimmer::clock

#pragma once

#include "microsecond_duration.hpp"

namespace video_trimmer::clock {

class VideoDuration : public MicrosecondDuration {
public:
    VideoDuration() = default;
    explicit VideoDuration(std::chrono::microseconds us) : MicrosecondDuration{us} { }
};

}  // namespace video_trimmer::clock

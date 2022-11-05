#pragma once

#include "microsecond_duration.hpp"

namespace video_trimmer::clock {

class FrameDuration : public MicrosecondDuration {
public:
    FrameDuration() = default;
    explicit FrameDuration(std::chrono::microseconds us) : MicrosecondDuration{us} { }
};

}  // namespace video_trimmer::clock

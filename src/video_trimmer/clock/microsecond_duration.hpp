#pragma once

#include <chrono>

namespace video_trimmer::clock {

class MicrosecondDuration {
public:
    MicrosecondDuration() : us_{0} { }
    explicit MicrosecondDuration(std::chrono::microseconds us) : us_{us} { }

    auto operator<=>(const MicrosecondDuration&) const = default;

    bool operator==(const MicrosecondDuration& other) const { return us_ == other.us_; }
    bool operator==(const std::chrono::microseconds& us) const { return us_ == us; }

    [[nodiscard]] std::chrono::microseconds microseconds() const { return us_; }

private:
    std::chrono::microseconds us_;
};

}  // namespace video_trimmer::clock

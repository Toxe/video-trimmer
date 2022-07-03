#pragma once

#include <chrono>

namespace video_trimmer::clock {

class Duration {
    std::chrono::nanoseconds ns_;

public:
    Duration() : ns_{0} { }
    explicit Duration(const std::chrono::nanoseconds ns) : ns_{ns} { }

    [[nodiscard]] long long as_microseconds() const { return std::chrono::duration_cast<std::chrono::microseconds>(ns_).count(); }
    [[nodiscard]] float as_seconds() const { return std::chrono::duration<float>{ns_}.count(); }
    [[nodiscard]] float fps() const { return 1.0f / as_seconds(); }
};

}  // namespace video_trimmer::clock

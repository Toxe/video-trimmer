#pragma once

#include "video_trimmer/clock/clock.hpp"

namespace video_trimmer::app {

class App {
public:
    App();

    void begin_frame();

    [[nodiscard]] const clock::Clock& frame_time_clock() const { return frame_time_clock_; }

    [[nodiscard]] clock::Duration previous_frame_time() const { return previous_frame_time_; }
    void set_previous_frame_time(clock::Duration frame_time);

private:
    clock::Clock frame_time_clock_;
    clock::Duration previous_frame_time_;
};

}  // namespace video_trimmer::app

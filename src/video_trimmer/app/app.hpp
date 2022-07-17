#pragma once

#include "video_trimmer/clock/clock.hpp"

namespace video_trimmer::app {

class App {
public:
    App();

    [[nodiscard]] video_trimmer::clock::Duration elapsed_time() const { return elapsed_time_; };

    void begin_frame();

private:
    video_trimmer::clock::Clock frame_time_clock_;
    video_trimmer::clock::Duration elapsed_time_;
};

}  // namespace video_trimmer::app

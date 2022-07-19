#pragma once

#include "video_trimmer/clock/clock.hpp"

namespace video_trimmer::app {

class App {
public:
    App();

    void begin_frame();

    [[nodiscard]] clock::Duration current_frame_time() const { return frame_time_clock_.elapsed_time(); }
    [[nodiscard]] clock::Duration previous_frame_time() const { return previous_frame_time_; }

private:
    clock::Clock frame_time_clock_;
    clock::Duration previous_frame_time_;
};

}  // namespace video_trimmer::app

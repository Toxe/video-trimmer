#pragma once

#include "../../../clock/clock.hpp"
#include "../ring_buffer.hpp"

namespace video_trimmer::ui::widgets::frame_time_widget {

class FrameTimeWidget {
public:
    FrameTimeWidget();

    void render(clock::Duration frame_time);

private:
    widgets::RingBuffer values_;
};

}  // namespace video_trimmer::ui::widgets::frame_time_widget

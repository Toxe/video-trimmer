#pragma once

#include "../ring_buffer.hpp"
#include "video_trimmer/clock/duration.hpp"

namespace video_trimmer::ui::widgets::frame_time_widget {

class FrameTimeWidget {
public:
    FrameTimeWidget();

    void render(clock::Duration frame_time);

private:
    widgets::RingBuffer values_;
};

}  // namespace video_trimmer::ui::widgets::frame_time_widget

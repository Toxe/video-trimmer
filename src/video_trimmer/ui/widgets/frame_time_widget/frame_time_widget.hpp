#pragma once

#include <vector>

#include "video_trimmer/clock/duration.hpp"

namespace video_trimmer::ui::widgets::frame_time_widget {

class FrameTimeWidget {
public:
    FrameTimeWidget();

    void render(clock::Duration frame_time);

private:
    std::vector<float> values_;
    std::size_t values_offset_ = 0;
};

}  // namespace video_trimmer::ui::widgets::frame_time_widget

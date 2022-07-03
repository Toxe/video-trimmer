#pragma once

#include <vector>

#include "video_trimmer/clock/duration.hpp"

namespace video_trimmer::ui::widgets::fps_widget {

class FPSWidget {
    float font_size_;

    std::vector<float> fps_;
    std::size_t values_offset_ = 0;

public:
    FPSWidget(float font_size);

    void render(video_trimmer::clock::Duration elapsed_time);
};

}  // namespace video_trimmer::ui::widgets::fps_widget

#pragma once

#include <vector>

#include "video_trimmer/clock/duration.hpp"

namespace video_trimmer::ui::widgets::fps_widget {

class FPSWidget {
public:
    FPSWidget();

    void render(video_trimmer::clock::Duration elapsed_time);

private:
    std::vector<float> fps_;
    std::size_t values_offset_ = 0;
};

}  // namespace video_trimmer::ui::widgets::fps_widget

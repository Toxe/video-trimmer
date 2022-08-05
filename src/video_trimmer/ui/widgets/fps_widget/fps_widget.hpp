#pragma once

#include <chrono>

#include "../ring_buffer.hpp"

namespace video_trimmer::ui::widgets::fps_widget {

class FPSWidget {
public:
    FPSWidget();

    void render();

private:
    widgets::RingBuffer fps_values_;

    float avg_fps_ = 0.0f;
    float avg_fps_accum_ = 0.0f;
    int avg_fps_count_ = 0;

    std::chrono::steady_clock::time_point last_avg_fps_update_time_;
};

}  // namespace video_trimmer::ui::widgets::fps_widget

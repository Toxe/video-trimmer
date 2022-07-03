#pragma once

#include <memory>
#include <vector>

#include "memory_usage_info/memory_usage_info.hpp"
#include "video_trimmer/clock/duration.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

class MemoryUsageWidget {
public:
    explicit MemoryUsageWidget(float font_size);

    void render(video_trimmer::clock::Duration elapsed_time);

private:
    std::unique_ptr<memory_usage_info::MemoryUsageInfo> memory_usage_;

    float font_size_;

    std::vector<float> mem_;
    std::size_t values_offset_ = 0;
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

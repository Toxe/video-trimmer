#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "memory_usage_info/memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

class MemoryUsageWidget {
public:
    MemoryUsageWidget();

    void render();

private:
    std::unique_ptr<memory_usage_info::MemoryUsageInfo> memory_usage_;

    std::vector<float> mem_;
    std::size_t values_offset_ = 0;

    float current_memory_usage_ = 0.0;

    std::chrono::steady_clock::time_point last_update_time_;
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

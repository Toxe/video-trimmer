#pragma once

#include <chrono>
#include <memory>

#include "../ring_buffer.hpp"
#include "memory_usage_info/memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

class MemoryUsageWidget {
public:
    MemoryUsageWidget();

    void render();

private:
    std::unique_ptr<memory_usage_info::MemoryUsageInfo> memory_usage_;

    widgets::RingBuffer values_;

    std::chrono::steady_clock::time_point last_update_time_;
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

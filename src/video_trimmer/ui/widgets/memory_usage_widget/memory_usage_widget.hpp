#pragma once

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
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

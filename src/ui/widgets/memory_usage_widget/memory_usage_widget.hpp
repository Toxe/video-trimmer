#pragma once

#include <memory>
#include <vector>

#include "memory_usage_info/memory_usage_info.hpp"

class Duration;

class MemoryUsageWidget {
public:
    explicit MemoryUsageWidget(float font_size);

    void render(Duration elapsed_time);

private:
    std::unique_ptr<MemoryUsageInfo> memory_usage_;

    float font_size_;

    std::vector<float> mem_;
    std::size_t values_offset_ = 0;
};

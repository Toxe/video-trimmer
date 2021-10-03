#pragma once

#include <vector>

class Duration;

class FPSView {
    float font_size_;

    std::vector<float> fps_;
    std::size_t values_offset_ = 0;

public:
    FPSView(const float font_size);

    void render(const Duration elapsed_time);
};

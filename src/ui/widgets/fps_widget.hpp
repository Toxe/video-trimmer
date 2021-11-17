#pragma once

#include <vector>

class Duration;

class FPSWidget {
    float font_size_;

    std::vector<float> fps_;
    std::size_t values_offset_ = 0;

public:
    FPSWidget(float font_size);

    void render(Duration elapsed_time);
};

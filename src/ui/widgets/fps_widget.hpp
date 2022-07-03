#pragma once

#include <vector>

#include "clock/duration.h"

class FPSWidget {
    float font_size_;

    std::vector<float> fps_;
    std::size_t values_offset_ = 0;

public:
    FPSWidget(float font_size);

    void render(Duration elapsed_time);
};

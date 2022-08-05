#pragma once

#include <vector>

namespace video_trimmer::ui::widgets {

class RingBuffer {
public:
    explicit RingBuffer(int capacity);

    void push(float value);

    [[nodiscard]] float get(int position) const;
    [[nodiscard]] float last() const;

    [[nodiscard]] int size() const { return static_cast<int>(values_.size()); }
    [[nodiscard]] int offset() const { return static_cast<int>(values_offset_); }

    [[nodiscard]] float max() const;

    [[nodiscard]] const float* data() const { return values_.data(); }

private:
    std::vector<float> values_;
    std::size_t values_offset_ = 0;
};

}  // namespace video_trimmer::ui::widgets

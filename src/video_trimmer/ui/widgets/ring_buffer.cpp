#include "ring_buffer.hpp"

#include <algorithm>
#include <stdexcept>

namespace video_trimmer::ui::widgets {

RingBuffer::RingBuffer(int capacity)
{
    if (capacity <= 0)
        throw std::invalid_argument("invalid capacity");

    values_.resize(static_cast<std::size_t>(capacity), 0.0f);
}

void RingBuffer::push(float value)
{
    values_[values_offset_] = value;
    values_offset_ = (values_offset_ + 1) % values_.size();
}

float RingBuffer::get(int position) const
{
    return values_[static_cast<std::size_t>(position)];
}

float RingBuffer::last() const
{
    if (values_offset_ == 0)
        return values_[values_.size() - 1];
    else
        return values_[(values_offset_ - 1) % values_.size()];
}

float RingBuffer::max() const
{
    return *std::max_element(values_.begin(), values_.end());
}

}  // namespace video_trimmer::ui::widgets

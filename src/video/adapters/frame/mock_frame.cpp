#include "mock_frame.hpp"

MockFrame::MockFrame(CodecContext* codec_context, const int scaled_width, const int scaled_height)
    : Frame(codec_context, scaled_width, scaled_height)
{
}

std::span<const uint8_t> MockFrame::pixels()
{
    return std::span<const uint8_t>{};
}

void MockFrame::image_copy()
{
}

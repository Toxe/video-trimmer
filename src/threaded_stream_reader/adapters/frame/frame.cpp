#include "frame.hpp"

#include "../codec_context/codec_context.hpp"

Frame::Frame(CodecContext* codec_context, const int scaled_width, const int scaled_height)
{
    src_width_ = codec_context->width();
    src_height_ = codec_context->height();

    // only scale down, never scale frames up to be bigger than the source frame
    if (scaled_width <= src_width_ && scaled_height <= src_height_) {
        dst_width_ = scaled_width;
        dst_height_ = scaled_height;
    } else {
        dst_width_ = src_width_;
        dst_height_ = src_height_;
    }
}

double Frame::timestamp() const
{
    return timestamp_;
}

void Frame::set_timestamp(const double timestamp)
{
    timestamp_ = timestamp;
}

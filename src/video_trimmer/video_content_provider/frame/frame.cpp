#include "frame.hpp"

#include <stdexcept>

extern "C" {
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/mem.h"
}

namespace video_trimmer::video_content_provider::frame {

Frame::Frame(const int width, const int height, const int scaled_width, const int scaled_height, AVPixelFormat pixel_format)
    : src_width_(width), src_height_(height), src_pixel_format_(pixel_format)
{
    // only scale down, never scale frames up to be bigger than the source frame
    if (scaled_width <= src_width_ && scaled_height <= src_height_) {
        dst_width_ = scaled_width;
        dst_height_ = scaled_height;
    } else {
        dst_width_ = src_width_;
        dst_height_ = src_height_;
    }

    frame_ = AutoDeleteResource<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });

    if (!frame_)
        throw std::runtime_error("av_frame_alloc");

    // allocate buffer for decoded source images
    int buf_size = av_image_alloc(img_buf_data_.data(), img_buf_linesize_.data(), src_width(), src_height(), src_pixel_format_, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");

    // allocate buffer for scaled output images
    buf_size = av_image_alloc(dst_buf_data_.data(), dst_buf_linesize_.data(), dst_width(), dst_height(), AV_PIX_FMT_RGBA, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");
}

Frame::~Frame()
{
    av_freep(dst_buf_data_.data());
    av_freep(img_buf_data_.data());
}

std::span<const uint8_t> Frame::pixels()
{
    return std::span<const uint8_t>{dst_buf_data_[0], static_cast<std::size_t>(4 * dst_width() * dst_height())};
}

void Frame::image_copy()
{
    av_image_copy(src_data(), src_linesizes(), const_cast<const uint8_t**>(frame_->data), &frame_->linesize[0], src_pixel_format_, src_width(), src_height());
    av_frame_unref(frame_.get());
}

}  // namespace video_trimmer::video_content_provider::frame
#include "video_reader.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_reader {

VideoReader::VideoReader(video_file::VideoFile& video_file, int scale_width, int scale_height)
    : video_file_(video_file),
      scale_width_(scale_width),
      scale_height_(scale_height)
{
    scaling_context_ = std::make_unique<scaling_context::ScalingContext>(video_file_.video_codec_context(), scale_width, scale_height);
}

std::unique_ptr<frame::Frame> VideoReader::read_next_frame(const double playback_position)
{
    std::unique_ptr<frame::Frame> frame = video_file_.read_next_frame(playback_position, scale_width_, scale_height_);

    if (frame && frame->is_video_frame())
        scale_frame(frame.get());

    return frame;
}

void VideoReader::scale_frame(frame::Frame* frame)
{
    // convert to destination format
    if (scale_width_ != frame->dst_width() || scale_height_ != frame->dst_height())
        resize_scaling_context(frame->dst_width(), frame->dst_height());

    if (scaling_context_)
        scaling_context_->scale(frame);
}

void VideoReader::resize_scaling_context(int width, int height)
{
    if (width != scale_width_ || height != scale_height_) {
        video_trimmer::logger::log_debug(fmt::format("(VideoFrameScaler) resize scaling context to {}x{}", width, height));

        scale_width_ = width;
        scale_height_ = height;

        scaling_context_ = std::make_unique<scaling_context::ScalingContext>(video_file_.video_codec_context(), width, height);
    }
}

void VideoReader::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    resize_scaling_context(scale_width, scale_height);
}

}  // namespace video_trimmer::video_reader

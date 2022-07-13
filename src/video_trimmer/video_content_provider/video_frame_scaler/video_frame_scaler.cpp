#include "video_frame_scaler.hpp"

#include <stdexcept>

#include "fmt/core.h"

#include "video_trimmer/error/error.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_content_provider::video_frame_scaler {

VideoFrameScaler::VideoFrameScaler(stream_info::StreamInfo* video_stream_info, int width, int height)
    : video_stream_info_(video_stream_info), scale_height_(height), scale_width_(width)
{
    scaling_context_ = std::make_unique<ScalingContext>(video_stream_info_->codec_context(), width, height);

    if (!scaling_context_)
        throw std::runtime_error("create_scaling_context");
}

void VideoFrameScaler::scale_frame(frame::Frame* frame)
{
    // convert to destination format
    if (scale_width_ != frame->dst_width() || scale_height_ != frame->dst_height())
        resize_scaling_context(frame->dst_width(), frame->dst_height());

    if (scaling_context_)
        scaling_context_->scale(frame);
}

int VideoFrameScaler::resize_scaling_context(int width, int height)
{
    video_trimmer::logger::log_debug(fmt::format("(VideoFrameScaler) resize scaling context to {}x{}", width, height));

    scale_width_ = width;
    scale_height_ = height;

    scaling_context_ = std::make_unique<ScalingContext>(video_stream_info_->codec_context(), width, height);

    if (!scaling_context_)
        return video_trimmer::error::show_error("create_scaling_context");

    return 0;
}

}  // namespace video_trimmer::video_content_provider::video_frame_scaler

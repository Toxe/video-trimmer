#include "video_frame_scaler.hpp"

#include <stdexcept>

#include "fmt/core.h"

#include "video_trimmer/error/error.hpp"
#include "video_trimmer/logger/logger.hpp"

VideoFrameScaler::VideoFrameScaler(StreamInfo* video_stream_info, int width, int height)
    : video_stream_info_(video_stream_info), scale_height_(height), scale_width_(width)
{
    scaling_context_ = std::make_unique<ScalingContext>(video_stream_info_->codec_context(), width, height);

    if (!scaling_context_)
        throw std::runtime_error("create_scaling_context");
}

void VideoFrameScaler::scale_frame(VideoFrame* video_frame)
{
    // convert to destination format
    if (scale_width_ != video_frame->width() || scale_height_ != video_frame->height())
        resize_scaling_context(video_frame->width(), video_frame->height());

    if (scaling_context_)
        scaling_context_->scale(video_frame);
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

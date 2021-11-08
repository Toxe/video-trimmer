#include "ffmpeg_scaling_context.hpp"

#include <stdexcept>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

#include "../../video_frame/video_frame.hpp"
#include "error/error.hpp"

FFmpegScalingContext::FFmpegScalingContext(CodecContext* codec_context, const int width, const int height)
    : ScalingContext(codec_context, width, height)
{
    scaling_context_ = auto_delete_ressource<SwsContext>(sws_getContext(src_width_, src_height_, src_pixel_format_, dst_width_, dst_height_, dst_pixel_format_, SWS_BILINEAR, nullptr, nullptr, nullptr), [](SwsContext* ctx) { sws_freeContext(ctx); });

    if (!scaling_context_)
        throw std::runtime_error("sws_getContext");
}

int FFmpegScalingContext::scale(VideoFrame* video_frame)
{
    int ret = sws_scale(scaling_context_.get(), video_frame->frame()->src_data(), video_frame->frame()->src_linesizes(), 0, src_height_, video_frame->frame()->dst_data(), video_frame->frame()->dst_linesizes());

    if (ret != dst_height_)
        show_error("sws_scale");

    return ret;
}

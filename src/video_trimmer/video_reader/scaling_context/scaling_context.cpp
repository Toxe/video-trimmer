#include "scaling_context.hpp"

#include <stdexcept>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
}

#include "auto_delete_resource.hpp"
#include "video_trimmer/error/error.hpp"

namespace video_trimmer::video_reader::scaling_context {

class ScalingContext::Impl {
public:
    Impl(codec_context::CodecContext* codec_context, int width, int height);

    int scale(frame::Frame* frame);

private:
    AutoDeleteResource<SwsContext> scaling_context_;

    int src_width_;
    int src_height_;
    int dst_width_;
    int dst_height_;

    AVPixelFormat src_pixel_format_;
    AVPixelFormat dst_pixel_format_;
};

ScalingContext::Impl::Impl(codec_context::CodecContext* codec_context, int width, int height)
    : src_width_(codec_context->width()), src_height_(codec_context->height()), dst_width_(width), dst_height_(height), src_pixel_format_(codec_context->pixel_format()), dst_pixel_format_(AV_PIX_FMT_RGBA)
{
    scaling_context_ = AutoDeleteResource<SwsContext>(sws_getContext(src_width_, src_height_, src_pixel_format_, dst_width_, dst_height_, dst_pixel_format_, SWS_BILINEAR, nullptr, nullptr, nullptr), [](SwsContext* ctx) { sws_freeContext(ctx); });

    if (!scaling_context_)
        throw std::runtime_error("sws_getContext");
}

int ScalingContext::Impl::scale(frame::Frame* frame)
{
    // int ret = sws_scale(scaling_context_.get(), frame->src_data(), frame->src_linesizes(), 0, src_height_, frame->dst_data(), frame->dst_linesizes());
    //
    // if (ret != dst_height_)
    //     video_trimmer::error::show_error("sws_scale");
    //
    // return ret;
    return 0;
}

ScalingContext::ScalingContext(codec_context::CodecContext* codec_context, int width, int height) : impl_(std::make_unique<ScalingContext::Impl>(codec_context, width, height)) { }
ScalingContext::~ScalingContext() = default;
int ScalingContext::scale(frame::Frame* frame) { return impl_->scale(frame); }

}  // namespace video_trimmer::video_reader::scaling_context

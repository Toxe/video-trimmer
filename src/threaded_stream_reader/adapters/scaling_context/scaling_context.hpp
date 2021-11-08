#pragma once

#include "../codec_context/codec_context.hpp"

extern "C" {
#include <libavutil/pixfmt.h>
}

class VideoFrame;

class ScalingContext {
protected:
    int src_width_, src_height_;
    int dst_width_, dst_height_;

    AVPixelFormat src_pixel_format_;
    AVPixelFormat dst_pixel_format_;

public:
    ScalingContext(CodecContext* codec_context, const int width, const int height)
    {
        src_width_ = codec_context->width();
        src_height_ = codec_context->height();

        dst_width_ = width;
        dst_height_ = height;

        src_pixel_format_ = codec_context->pixel_format();
        dst_pixel_format_ = AV_PIX_FMT_RGBA;
    }

    virtual ~ScalingContext() = default;

    virtual int scale(VideoFrame* video_frame) = 0;
};

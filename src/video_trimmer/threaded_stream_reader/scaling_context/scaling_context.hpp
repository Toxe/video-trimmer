#pragma once

extern "C" {
#include "libavutil/pixfmt.h"
}

#include "../codec_context/codec_context.hpp"
#include "../video_frame/video_frame.hpp"
#include "auto_delete_resource.hpp"

struct SwsContext;

class ScalingContext {
public:
    ScalingContext(CodecContext* codec_context, int width, int height);

    int scale(VideoFrame* video_frame);

private:
    AutoDeleteResource<SwsContext> scaling_context_;

    int src_width_, src_height_;
    int dst_width_, dst_height_;

    AVPixelFormat src_pixel_format_;
    AVPixelFormat dst_pixel_format_;
};
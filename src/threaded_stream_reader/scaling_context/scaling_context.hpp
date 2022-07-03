#pragma once

#include "auto_delete_resource.hpp"
#include "threaded_stream_reader/codec_context/codec_context.hpp"

extern "C" {
#include "libavutil/pixfmt.h"
}

class VideoFrame;
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

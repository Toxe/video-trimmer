#pragma once

extern "C" {
#include "libavutil/pixfmt.h"
}

#include "../codec_context/codec_context.hpp"
#include "../frame/frame.hpp"
#include "auto_delete_resource.hpp"

struct SwsContext;

namespace video_trimmer::video_reader::video_frame_scaler {

class ScalingContext {
public:
    ScalingContext(codec_context::CodecContext* codec_context, int width, int height);

    int scale(frame::Frame* frame);

private:
    AutoDeleteResource<SwsContext> scaling_context_;

    int src_width_, src_height_;
    int dst_width_, dst_height_;

    AVPixelFormat src_pixel_format_;
    AVPixelFormat dst_pixel_format_;
};

}  // namespace video_trimmer::video_reader::video_frame_scaler

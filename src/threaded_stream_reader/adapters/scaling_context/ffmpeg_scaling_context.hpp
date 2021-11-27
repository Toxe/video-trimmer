#pragma once

#include "scaling_context.hpp"

#include "auto_delete_resource.hpp"

struct SwsContext;

class FFmpegScalingContext : public ScalingContext {
    AutoDeleteResource<SwsContext> scaling_context_;

public:
    FFmpegScalingContext(CodecContext* codec_context, int width, int height);

    int scale(VideoFrame* video_frame) override;
};

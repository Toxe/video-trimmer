#pragma once

#include "scaling_context.hpp"

#include "auto_delete_resource.hpp"

struct SwsContext;

class FFmpegScalingContext : public ScalingContext {
    auto_delete_resource<SwsContext> scaling_context_ = {nullptr, nullptr};

public:
    FFmpegScalingContext(CodecContext* codec_context, int width, int height);

    int scale(VideoFrame* video_frame) override;
};

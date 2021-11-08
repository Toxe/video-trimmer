#pragma once

#include "scaling_context.hpp"

#include "auto_delete_ressource.hpp"

struct SwsContext;

class FFmpegScalingContext : public ScalingContext {
    auto_delete_ressource<SwsContext> scaling_context_ = {nullptr, nullptr};

public:
    FFmpegScalingContext(CodecContext* codec_context, const int width, const int height);

    virtual int scale(VideoFrame* video_frame) override;
};

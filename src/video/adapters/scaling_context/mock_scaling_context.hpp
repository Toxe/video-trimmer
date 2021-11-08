#pragma once

#include "scaling_context.hpp"

#include "auto_delete_ressource.hpp"

class MockScalingContext : public ScalingContext {
public:
    MockScalingContext(CodecContext* codec_context, const int width, const int height);

    virtual int scale(VideoFrame* video_frame) override;
};

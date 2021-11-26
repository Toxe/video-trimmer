#pragma once

#include "scaling_context.hpp"

#include "auto_delete_resource.hpp"

class MockScalingContext : public ScalingContext {
public:
    MockScalingContext(CodecContext* codec_context, int width, int height);

    int scale(VideoFrame* video_frame) override;
};

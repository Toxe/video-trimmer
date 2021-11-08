#include "mock_scaling_context.hpp"

#include <stdexcept>

#include "../../video_frame/video_frame.hpp"

MockScalingContext::MockScalingContext(CodecContext* codec_context, const int width, const int height)
    : ScalingContext(codec_context, width, height)
{
}

int MockScalingContext::scale(VideoFrame* /*video_frame*/)
{
    return 0;
}

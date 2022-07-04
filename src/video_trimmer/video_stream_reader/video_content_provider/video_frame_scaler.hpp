#pragma once

#include "../scaling_context/scaling_context.hpp"
#include "../stream_info/stream_info.hpp"
#include "../video_frame/video_frame.hpp"

class VideoFrameScaler {
public:
    VideoFrameScaler(StreamInfo* video_stream_info, int width, int height);

    void scale_frame(VideoFrame* video_frame);

private:
    std::unique_ptr<ScalingContext> scaling_context_;

    StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    int resize_scaling_context(int width, int height);
};

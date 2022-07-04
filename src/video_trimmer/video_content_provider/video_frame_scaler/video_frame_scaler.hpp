#pragma once

#include "../stream_info/stream_info.hpp"
#include "../video_frame/video_frame.hpp"
#include "scaling_context.hpp"

namespace video_trimmer::video_content_provider::video_frame_scaler {

class VideoFrameScaler {
public:
    VideoFrameScaler(stream_info::StreamInfo* video_stream_info, int width, int height);

    void scale_frame(video_frame::VideoFrame* video_frame);

private:
    std::unique_ptr<ScalingContext> scaling_context_;

    stream_info::StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    int resize_scaling_context(int width, int height);
};

}  // namespace video_trimmer::video_content_provider::video_frame_scaler

#pragma once

#include <latch>
#include <memory>
#include <queue>
#include <stop_token>

#include "../scaling_context/scaling_context.hpp"
#include "../stream_info/stream_info.hpp"
#include "../video_frame/video_frame.hpp"
#include "work_thread.hpp"

class VideoContentProvider;

class VideoFrameScaler : public WorkThread {
    std::unique_ptr<ScalingContext> scaling_context_;

    std::queue<std::unique_ptr<VideoFrame>> queue_;

    StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    void main(std::stop_token st, VideoContentProvider* video_content_provider, std::latch& latch) override;

    void scale_frame(VideoFrame* video_frame);
    int resize_scaling_context(int width, int height);

public:
    VideoFrameScaler(StreamInfo* video_stream_info, int width, int height);

    void add_to_queue(std::unique_ptr<VideoFrame> video_frame);
    [[nodiscard]] std::unique_ptr<VideoFrame> remove_from_queue();
    [[nodiscard]] int queue_size();
};
#pragma once

#include <latch>
#include <memory>
#include <optional>
#include <stop_token>

#include "../packet/packet.hpp"
#include "../stream_info/stream_info.hpp"
#include "work_thread.hpp"

class VideoContentProvider;

class VideoReader : public WorkThread {
    std::unique_ptr<Packet> packet_;

    StreamInfo* audio_stream_info_;
    StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    void main(std::stop_token st, VideoContentProvider* video_content_provider, std::latch& latch) override;

    [[nodiscard]] std::optional<std::unique_ptr<VideoFrame>> read();
    [[nodiscard]] std::unique_ptr<VideoFrame> decode_video_packet(Packet* packet);

public:
    VideoReader(StreamInfo* audio_stream_info, StreamInfo* video_stream_info, const int scale_width, const int scale_height);

    void change_scaling_dimensions(int scale_width, int scale_height);
};
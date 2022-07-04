#pragma once

#include <memory>

#include "../packet/packet.hpp"
#include "../stream_info/stream_info.hpp"

class VideoReader {
public:
    VideoReader(StreamInfo* audio_stream_info, StreamInfo* video_stream_info, int scale_width, int scale_height);

    void change_scaling_dimensions(int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<VideoFrame> read();

private:
    std::unique_ptr<Packet> packet_;

    StreamInfo* audio_stream_info_;
    StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    [[nodiscard]] std::unique_ptr<VideoFrame> decode_video_packet(Packet* packet);
};

#pragma once

#include <memory>

#include "../frame/frame.hpp"
#include "../packet/packet.hpp"
#include "../stream_info/stream_info.hpp"

namespace video_trimmer::video_content_provider::video_reader {

class VideoReader {
public:
    VideoReader(stream_info::StreamInfo* audio_stream_info, stream_info::StreamInfo* video_stream_info, int scale_width, int scale_height);

    void change_scaling_dimensions(int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<frame::Frame> read();

private:
    std::unique_ptr<packet::Packet> packet_;

    stream_info::StreamInfo* audio_stream_info_;
    stream_info::StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    [[nodiscard]] std::unique_ptr<frame::Frame> decode_video_packet(packet::Packet* packet);
};

}  // namespace video_trimmer::video_content_provider::video_reader

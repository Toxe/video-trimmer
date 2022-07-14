#pragma once

#include <memory>

#include "frame/frame.hpp"
#include "packet/packet.hpp"
#include "scaling_context/scaling_context.hpp"
#include "stream_info/stream_info.hpp"
#include "video_file/video_file.hpp"

namespace video_trimmer::video_reader {

class VideoReader {
public:
    VideoReader(video_file::VideoFile& video_file, int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position);

    void change_scaling_dimensions(int scale_width, int scale_height);

private:
    std::unique_ptr<packet::Packet> packet_;
    std::unique_ptr<scaling_context::ScalingContext> scaling_context_;

    stream_info::StreamInfo* audio_stream_info_;
    stream_info::StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    [[nodiscard]] std::unique_ptr<frame::Frame> decode_video_packet(packet::Packet* packet);

    void scale_frame(frame::Frame* frame);
    void resize_scaling_context(int width, int height);
};

}  // namespace video_trimmer::video_reader

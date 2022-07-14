#pragma once

#include <memory>

#include "video_file/video_file.hpp"
#include "video_frame_scaler/video_frame_scaler.hpp"
#include "video_trimmer/video_reader/frame/frame.hpp"
#include "video_trimmer/video_reader/packet/packet.hpp"
#include "video_trimmer/video_reader/stream_info/stream_info.hpp"

namespace video_trimmer::video_reader {

class VideoReader {
public:
    VideoReader(video_file::VideoFile& video_file, int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position);

    void change_scaling_dimensions(int scale_width, int scale_height);

private:
    video_frame_scaler::VideoFrameScaler video_frame_scaler_;

    std::unique_ptr<packet::Packet> packet_;

    stream_info::StreamInfo* audio_stream_info_;
    stream_info::StreamInfo* video_stream_info_;

    int scale_width_ = 0;
    int scale_height_ = 0;

    [[nodiscard]] std::unique_ptr<frame::Frame> decode_video_packet(packet::Packet* packet);
};

}  // namespace video_trimmer::video_reader

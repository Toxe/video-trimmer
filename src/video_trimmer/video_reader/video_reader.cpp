#include "video_reader.hpp"

namespace video_trimmer::video_reader {

VideoReader::VideoReader(video_file::VideoFile& video_file, int scale_width, int scale_height)
    : video_frame_scaler_{video_file.video_stream_info(), scale_width, scale_height},
      audio_stream_info_(video_file.audio_stream_info()),
      video_stream_info_(video_file.video_stream_info()),
      scale_width_(scale_width),
      scale_height_(scale_height)
{
    packet_ = std::make_unique<packet::Packet>();
}

std::unique_ptr<frame::Frame> VideoReader::read_next_frame(const double playback_position)
{
    // read until we get at least one video frame
    while (true) {
        if (video_stream_info_->format_context()->read_frame(packet_.get()) < 0)
            break;

        // process only interesting packets, drop the rest
        if (packet_->stream_index() == video_stream_info_->stream_index()) {
            std::unique_ptr<frame::Frame> frame = decode_video_packet(packet_.get());
            packet_->unref();

            if (frame)
                video_frame_scaler_.scale_frame(frame.get());

            return frame;
        } else if (packet_->stream_index() == audio_stream_info_->stream_index()) {
            // TODO: decode audio packet
            packet_->unref();
        } else {
            packet_->unref();
        }
    }

    return nullptr;
}

std::unique_ptr<frame::Frame> VideoReader::decode_video_packet(packet::Packet* packet)
{
    // send packet to the decoder
    if (video_stream_info_->codec_context()->send_packet(packet) < 0)
        return nullptr;

    // get available frame from the decoder
    return video_stream_info_->receive_video_frame(scale_width_, scale_height_);
}

void VideoReader::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    scale_width_ = scale_width;
    scale_height_ = scale_height;
}

}  // namespace video_trimmer::video_reader

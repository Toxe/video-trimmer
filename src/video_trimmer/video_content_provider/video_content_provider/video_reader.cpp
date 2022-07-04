#include "video_reader.hpp"

VideoReader::VideoReader(StreamInfo* audio_stream_info, StreamInfo* video_stream_info, const int scale_width, const int scale_height)
    : audio_stream_info_(audio_stream_info), video_stream_info_(video_stream_info), scale_width_(scale_width), scale_height_(scale_height)
{
    packet_ = std::make_unique<Packet>();
}

std::unique_ptr<VideoFrame> VideoReader::read()
{
    // read until we get at least one video frame
    while (true) {
        if (video_stream_info_->format_context()->read_frame(packet_.get()) < 0)
            break;

        // process only interesting packets, drop the rest
        if (packet_->stream_index() == video_stream_info_->stream_index()) {
            std::unique_ptr<VideoFrame> video_frame = decode_video_packet(packet_.get());
            packet_->unref();
            return video_frame;
        } else if (packet_->stream_index() == audio_stream_info_->stream_index()) {
            // TODO: decode audio packet
            packet_->unref();
        } else {
            packet_->unref();
        }
    }

    return nullptr;
}

std::unique_ptr<VideoFrame> VideoReader::decode_video_packet(Packet* packet)
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

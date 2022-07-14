#include "video_reader.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_reader {

VideoReader::VideoReader(video_file::VideoFile& video_file, int scale_width, int scale_height)
    : audio_stream_info_(video_file.audio_stream_info()),
      video_stream_info_(video_file.video_stream_info()),
      scale_width_(scale_width),
      scale_height_(scale_height)
{
    packet_ = std::make_unique<packet::Packet>();
    scaling_context_ = std::make_unique<scaling_context::ScalingContext>(video_stream_info_->codec_context(), scale_width, scale_height);
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
                scale_frame(frame.get());

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

void VideoReader::scale_frame(frame::Frame* frame)
{
    // convert to destination format
    if (scale_width_ != frame->dst_width() || scale_height_ != frame->dst_height())
        resize_scaling_context(frame->dst_width(), frame->dst_height());

    if (scaling_context_)
        scaling_context_->scale(frame);
}

void VideoReader::resize_scaling_context(int width, int height)
{
    if (width != scale_width_ || height != scale_height_) {
        video_trimmer::logger::log_debug(fmt::format("(VideoFrameScaler) resize scaling context to {}x{}", width, height));

        scale_width_ = width;
        scale_height_ = height;

        scaling_context_ = std::make_unique<scaling_context::ScalingContext>(video_stream_info_->codec_context(), width, height);
    }
}

void VideoReader::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    resize_scaling_context(scale_width, scale_height);
}

}  // namespace video_trimmer::video_reader

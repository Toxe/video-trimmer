#include "video_reader.hpp"

#include "../adapters/format_context/format_context.hpp"
#include "../adapters/packet/packet.hpp"
#include "../factory/factory.hpp"
#include "../stream_info/stream_info.hpp"
#include "../video_frame/video_frame.hpp"
#include "error/error.hpp"
#include "logger/logger.hpp"
#include "video_content_provider.hpp"

VideoReader::VideoReader(Factory* factory, StreamInfo* audio_stream_info, StreamInfo* video_stream_info, const int scale_width, const int scale_height)
    : WorkThread{factory, "VideoReader"}
{
    audio_stream_info_ = audio_stream_info;
    video_stream_info_ = video_stream_info;

    scale_width_ = scale_width;
    scale_height_ = scale_height;

    packet_ = factory->create_packet();
}

void VideoReader::main(std::stop_token st, VideoContentProvider* video_content_provider, std::latch& latch)
{
    log_debug("(VideoReader) starting");

    latch.arrive_and_wait();

    set_state(RunState::running);

    const auto queue_not_full = [&] { return video_content_provider->finished_video_frames_queue_is_not_full(); };
    const auto stop_condition = [&] { return queue_not_full(); };

    while (!st.stop_requested()) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, st, stop_condition);
        }

        if (!st.stop_requested()) {
            auto video_frame = read();

            if (!video_frame.has_value())
                break;

            if (video_frame.value())
                video_content_provider->add_video_frame_for_scaling(std::move(video_frame.value()));
        }
    }

    set_state(RunState::fnished);

    log_debug("(VideoReader) stopping");
}

std::optional<std::unique_ptr<VideoFrame>> VideoReader::read()
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

    return std::nullopt;
}

std::unique_ptr<VideoFrame> VideoReader::decode_video_packet(Packet* packet)
{
    // send packet to the decoder
    if (video_stream_info_->codec_context()->send_packet(packet) < 0)
        return nullptr;

    // get available frame from the decoder
    return video_stream_info_->receive_video_frame(factory(), scale_width_, scale_height_);
}

void VideoReader::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    scale_width_ = scale_width;
    scale_height_ = scale_height;
}

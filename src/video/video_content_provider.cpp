#include "video_content_provider.hpp"

#include <spdlog/spdlog.h>

#include "audio_stream.hpp"
#include "error.hpp"
#include "video_stream.hpp"

VideoContentProvider::VideoContentProvider(AVFormatContext* format_context, VideoStream& video_stream, AudioStream& audio_stream)
    : format_context_{format_context}, video_stream_{video_stream}, audio_stream_{audio_stream}
{
    spdlog::trace("init VideoContentProvider");

    is_ready_ = init() == 0;
}

int VideoContentProvider::init()
{
    packet_ = auto_delete_ressource<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });

    if (!packet_)
        return show_error("av_packet_alloc");

    return 0;
}

bool VideoContentProvider::read(ImageSize video_size)
{
    // read until we get at least one video frame
    bool running = true;

    while (running) {
        int ret = av_read_frame(format_context_, packet_.get());

        if (ret < 0)
            return false;

        // process only interesting packets, skip the rest
        if (packet_->stream_index == video_stream_.stream_index()) {
            ret = video_stream_.decode_packet(packet_.get(), video_size);
            running = false;
        } else if (packet_->stream_index == audio_stream_.stream_index()) {
            ret = audio_stream_.decode_packet(packet_.get());
        }

        av_packet_unref(packet_.get());

        if (ret < 0)
            return false;
    }

    return true;
}

const uint8_t* VideoContentProvider::next_video_frame()
{
    if (!is_ready_)
        return nullptr;

    return video_stream_.next_frame();
}

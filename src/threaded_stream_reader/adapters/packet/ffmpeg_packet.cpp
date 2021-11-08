#include "ffmpeg_packet.hpp"

#include <stdexcept>

extern "C" {
#include <libavcodec/packet.h>
}

FFmpegPacket::FFmpegPacket()
{
    packet_ = auto_delete_ressource<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });

    if (!packet_)
        throw std::runtime_error("av_packet_alloc");
}

AVPacket* FFmpegPacket::packet() const
{
    return packet_.get();
}

int FFmpegPacket::stream_index() const
{
    return packet_->stream_index;
}

void FFmpegPacket::unref()
{
    av_packet_unref(packet_.get());
}

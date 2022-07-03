#include "packet.hpp"

#include <stdexcept>

extern "C" {
#include "libavcodec/packet.h"
}

Packet::Packet()
{
    packet_ = AutoDeleteResource<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });

    if (!packet_)
        throw std::runtime_error("av_packet_alloc");
}

AVPacket* Packet::packet() const
{
    return packet_.get();
}

int Packet::stream_index() const
{
    return packet_->stream_index;
}

void Packet::unref()
{
    av_packet_unref(packet_.get());
}

#pragma once

#include "auto_delete_resource.hpp"

struct AVPacket;

namespace video_trimmer::video_content_provider::packet {

class Packet {
public:
    Packet();

    [[nodiscard]] int stream_index() const;

    [[nodiscard]] AVPacket* packet() const;
    void unref();

private:
    AutoDeleteResource<AVPacket> packet_;
};

}  // namespace video_trimmer::video_content_provider::packet

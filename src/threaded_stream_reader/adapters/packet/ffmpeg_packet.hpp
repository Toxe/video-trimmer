#pragma once

#include "packet.hpp"

#include "auto_delete_resource.hpp"

class FFmpegPacket : public Packet {
    AutoDeleteResource<AVPacket> packet_;

public:
    FFmpegPacket();

    [[nodiscard]] int stream_index() const override;

    [[nodiscard]] AVPacket* packet() const override;
    void unref() override;
};

#pragma once

#include "packet.hpp"

#include "auto_delete_resource.hpp"

class FFmpegPacket : public Packet {
    auto_delete_resource<AVPacket> packet_ = {nullptr, nullptr};

public:
    FFmpegPacket();

    [[nodiscard]] int stream_index() const override;

    [[nodiscard]] AVPacket* packet() const override;
    void unref() override;
};

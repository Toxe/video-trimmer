#pragma once

#include "packet.hpp"

#include "auto_delete_ressource.hpp"

class FFmpegPacket : public Packet {
    auto_delete_ressource<AVPacket> packet_ = {nullptr, nullptr};

public:
    FFmpegPacket();

    [[nodiscard]] virtual int stream_index() const override;

    [[nodiscard]] virtual AVPacket* packet() const override;
    virtual void unref() override;
};

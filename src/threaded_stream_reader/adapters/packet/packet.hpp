#pragma once

#include "auto_delete_resource.hpp"

struct AVPacket;

class Packet {
public:
    Packet();

    [[nodiscard]] int stream_index() const;

    [[nodiscard]] AVPacket* packet() const;
    void unref();

private:
    AutoDeleteResource<AVPacket> packet_;
};

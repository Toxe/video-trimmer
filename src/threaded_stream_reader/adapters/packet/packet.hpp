#pragma once

struct AVPacket;

class Packet {
public:
    virtual ~Packet() = default;

    [[nodiscard]] virtual int stream_index() const = 0;
    virtual void set_stream_index(const int) { }

    [[nodiscard]] virtual AVPacket* packet() const = 0;
    virtual void unref() = 0;
};

#pragma once

#include "packet.hpp"

class MockPacket : public Packet {
    int stream_index_ = 0;

public:
    [[nodiscard]] virtual int stream_index() const override;
    virtual void set_stream_index(const int new_stream_index) override;

    [[nodiscard]] virtual AVPacket* packet() const override;
    virtual void unref() override;
};

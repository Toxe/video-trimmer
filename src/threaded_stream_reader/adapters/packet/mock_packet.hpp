#pragma once

#include "packet.hpp"

class MockPacket : public Packet {
    int stream_index_ = 0;

public:
    [[nodiscard]] int stream_index() const override;
    void set_stream_index(int new_stream_index) override;

    [[nodiscard]] AVPacket* packet() const override;
    void unref() override;
};

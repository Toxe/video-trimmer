#pragma once

#include "codec_context.hpp"

class MockCodecContext : public CodecContext {
    int next_frame_number_ = 0;

public:
    [[nodiscard]] std::string codec_type() override;
    [[nodiscard]] std::string codec_name() override;
    [[nodiscard]] std::string codec_additional_info() override;

    [[nodiscard]] int width() const override;
    [[nodiscard]] int height() const override;
    [[nodiscard]] AVPixelFormat pixel_format() const override;

    [[nodiscard]] virtual int send_packet(Packet* packet) override;
    [[nodiscard]] virtual std::unique_ptr<Frame> receive_frame(Factory* factory, double time_base, int scaled_width, int scaled_height) override;
};

#pragma once

#include "factory.hpp"

class MockFactory : public Factory {
public:
    [[nodiscard]] std::unique_ptr<CodecContext> create_codec_context(AVStream* stream) override;
    [[nodiscard]] std::unique_ptr<FormatContext> create_format_context(const std::string_view& filename) override;
    [[nodiscard]] std::unique_ptr<ScalingContext> create_scaling_context(CodecContext* codec_context, const int width, const int height) override;

    [[nodiscard]] std::unique_ptr<Frame> create_frame(CodecContext* codec_context, const int scaled_width, const int scaled_height) override;
    [[nodiscard]] std::unique_ptr<Packet> create_packet() override;
};

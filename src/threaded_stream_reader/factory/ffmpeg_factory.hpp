#pragma once

#include "factory.hpp"

class FFmpegFactory : public Factory {
public:
    [[nodiscard]] std::unique_ptr<CodecContext> create_codec_context(AVStream* stream) override;
    [[nodiscard]] std::unique_ptr<FormatContext> create_format_context(const std::string_view& filename) override;

    [[nodiscard]] std::unique_ptr<Frame> create_frame(CodecContext* codec_context, int scaled_width, int scaled_height) override;
};

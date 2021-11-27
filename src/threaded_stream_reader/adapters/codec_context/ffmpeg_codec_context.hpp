#pragma once

#include "auto_delete_resource.hpp"
#include "codec_context.hpp"

struct AVCodecContext;
struct AVStream;

class FFmpegCodecContext : public CodecContext {
    AutoDeleteResource<AVCodecContext> codec_context_;

    float fps_;

public:
    FFmpegCodecContext(AVStream* stream);

    [[nodiscard]] std::string codec_type() override;
    [[nodiscard]] std::string codec_name() override;
    [[nodiscard]] std::string codec_additional_info() override;

    [[nodiscard]] int width() const override;
    [[nodiscard]] int height() const override;
    [[nodiscard]] AVPixelFormat pixel_format() const override;
    [[nodiscard]] float fps() const override;

    [[nodiscard]] int send_packet(Packet* packet) override;
    [[nodiscard]] std::unique_ptr<Frame> receive_frame(Factory* factory, double time_base, int scaled_width, int scaled_height) override;
};

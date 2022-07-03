#pragma once

#include <memory>
#include <string>

extern "C" {
#include <libavutil/pixfmt.h>
}

#include "auto_delete_resource.hpp"

struct AVCodecContext;
struct AVStream;

class Frame;
class Packet;

class CodecContext {
public:
    CodecContext(AVStream* stream);

    [[nodiscard]] std::string codec_type();
    [[nodiscard]] std::string codec_name();
    [[nodiscard]] std::string codec_additional_info();

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] AVPixelFormat pixel_format() const;
    [[nodiscard]] float fps() const { return fps_; };

    [[nodiscard]] int send_packet(Packet* packet);
    [[nodiscard]] std::unique_ptr<Frame> receive_frame(const double time_base, const int scaled_width, const int scaled_height);

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    float fps_;
};

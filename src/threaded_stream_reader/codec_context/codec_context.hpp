#pragma once

#include <memory>
#include <string>

#include "../frame/frame.hpp"
#include "../packet/packet.hpp"
#include "auto_delete_resource.hpp"

struct AVCodecContext;
struct AVStream;

class CodecContext {
public:
    explicit CodecContext(AVStream* stream);

    [[nodiscard]] std::string codec_type();
    [[nodiscard]] std::string codec_name();
    [[nodiscard]] std::string codec_additional_info();

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] AVPixelFormat pixel_format() const;
    [[nodiscard]] float fps() const { return fps_; };

    [[nodiscard]] int send_packet(Packet* packet);
    [[nodiscard]] std::unique_ptr<Frame> receive_frame(double time_base, int scaled_width, int scaled_height);

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    float fps_;
};

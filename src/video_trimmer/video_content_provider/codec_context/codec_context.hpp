#pragma once

#include <memory>
#include <string>

#include "../frame/frame.hpp"
#include "../packet/packet.hpp"
#include "auto_delete_resource.hpp"

struct AVCodecContext;
struct AVStream;

namespace video_trimmer::video_content_provider::codec_context {

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

    [[nodiscard]] int send_packet(packet::Packet* packet);
    [[nodiscard]] std::unique_ptr<frame::Frame> receive_frame(double time_base, int scaled_width, int scaled_height);

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    float fps_;
};

}  // namespace video_trimmer::video_content_provider::codec_context

#pragma once

#include <memory>
#include <string>

#include "../frame/frame.hpp"
#include "../packet/packet.hpp"
#include "auto_delete_resource.hpp"

struct AVCodecContext;
struct AVStream;

namespace video_trimmer::video_reader::codec_context {

class CodecContext {
public:
    explicit CodecContext(AVStream* stream);

    [[nodiscard]] const std::string& codec_type() const { return codec_type_; }
    [[nodiscard]] const std::string& codec_name() const { return codec_name_; }
    [[nodiscard]] const std::string& codec_additional_info() const { return codec_additional_info_; }

    [[nodiscard]] int width() const;
    [[nodiscard]] int height() const;
    [[nodiscard]] AVPixelFormat pixel_format() const;
    [[nodiscard]] float fps() const { return fps_; };

    [[nodiscard]] int send_packet_to_decoder(packet::Packet* packet);
    [[nodiscard]] std::unique_ptr<frame::Frame> receive_frame_from_decoder(double time_base, int scaled_width, int scaled_height);

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    std::string codec_type_;
    std::string codec_name_;
    std::string codec_additional_info_;

    float fps_;
};

}  // namespace video_trimmer::video_reader::codec_context

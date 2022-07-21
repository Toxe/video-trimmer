#pragma once

#include <memory>
#include <string>

#include "../frame/frame.hpp"
#include "auto_delete_resource.hpp"
#include "types.hpp"

struct AVCodecContext;
struct AVPacket;
struct AVStream;

namespace video_trimmer::video_reader::codec_context {

class CodecContext {
public:
    enum class StreamType {
        audio,
        video
    };

    explicit CodecContext(AVStream* stream);

    [[nodiscard]] const std::string& codec_type() const { return codec_type_; }
    [[nodiscard]] const std::string& codec_name() const { return codec_name_; }
    [[nodiscard]] const std::string& codec_additional_info() const { return codec_additional_info_; }

    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] Size size() const;
    [[nodiscard]] AVPixelFormat pixel_format() const;

    [[nodiscard]] float fps() const { return fps_; };
    [[nodiscard]] double stream_time_base() const { return stream_time_base_; };

    [[nodiscard]] int send_packet_to_decoder(AVPacket* packet);
    [[nodiscard]] std::unique_ptr<frame::Frame> receive_frame_from_decoder(double time_base);

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    std::string codec_type_;
    std::string codec_name_;
    std::string codec_additional_info_;

    int stream_index_;

    float fps_;
    double stream_time_base_;
};

}  // namespace video_trimmer::video_reader::codec_context

#pragma once

#include <memory>
#include <string>

#include "auto_delete_resource.hpp"
#include "frame.hpp"
#include "types.hpp"

struct AVCodecContext;
struct AVFormatContext;
struct AVPacket;
struct AVStream;

namespace video_trimmer::video_file {

class CodecContext {
public:
    enum class StreamType {
        audio,
        video
    };

    explicit CodecContext(const AVStream* stream, const AVFormatContext* format_context);

    [[nodiscard]] const std::string& codec_type() const { return codec_type_; }
    [[nodiscard]] const std::string& codec_name() const { return codec_name_; }
    [[nodiscard]] const std::string& codec_additional_info() const { return codec_additional_info_; }

    [[nodiscard]] bool is_audio_stream() const;
    [[nodiscard]] bool is_video_stream() const;

    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] Size size() const;
    [[nodiscard]] PixelFormat pixel_format() const;

    [[nodiscard]] float fps() const { return fps_; };

    [[nodiscard]] int send_packet_to_decoder(AVPacket* packet);
    [[nodiscard]] std::unique_ptr<Frame> receive_frame_from_decoder();

    void flush_buffers();

private:
    AutoDeleteResource<AVCodecContext> codec_context_;

    std::string codec_type_;
    std::string codec_name_;
    std::string codec_additional_info_;

    int stream_index_;

    float fps_;
    int64_t stream_duration_;
    double stream_time_base_;
};

}  // namespace video_trimmer::video_file

#pragma once

#include <memory>
#include <string>

#include "../adapters/codec_context/codec_context.hpp"

class Factory;
class FormatContext;
class VideoFrame;

class StreamInfo {
    FormatContext* format_context_ = nullptr;
    std::unique_ptr<CodecContext> codec_context_;
    int stream_index_ = -1;

    std::string codec_type_;
    std::string codec_name_;
    std::string codec_additional_info_;

public:
    StreamInfo(FormatContext* format_context, std::unique_ptr<CodecContext> codec_context, int stream_index);

    [[nodiscard]] FormatContext* format_context() const { return format_context_; }
    [[nodiscard]] CodecContext* codec_context() const { return codec_context_.get(); }
    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] double time_base() const;

    [[nodiscard]] const std::string& codec_type() const { return codec_type_; }
    [[nodiscard]] const std::string& codec_name() const { return codec_name_; }
    [[nodiscard]] const std::string& codec_additional_info() const { return codec_additional_info_; }

    [[nodiscard]] std::unique_ptr<VideoFrame> receive_video_frame(Factory* factory, const int scaled_width, const int scaled_height);
};

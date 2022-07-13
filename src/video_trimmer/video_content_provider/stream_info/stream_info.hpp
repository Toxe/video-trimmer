#pragma once

#include <memory>
#include <string>

#include "../codec_context/codec_context.hpp"
#include "../format_context/format_context.hpp"
#include "../video_frame/video_frame.hpp"

namespace video_trimmer::video_content_provider::stream_info {

class StreamInfo {
public:
    StreamInfo(format_context::FormatContext* format_context, std::unique_ptr<codec_context::CodecContext> codec_context, int stream_index);

    [[nodiscard]] format_context::FormatContext* format_context() const { return format_context_; }
    [[nodiscard]] codec_context::CodecContext* codec_context() const { return codec_context_.get(); }
    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] double time_base() const;

    [[nodiscard]] const std::string& codec_type() const { return codec_type_; }
    [[nodiscard]] const std::string& codec_name() const { return codec_name_; }
    [[nodiscard]] const std::string& codec_additional_info() const { return codec_additional_info_; }

    [[nodiscard]] std::unique_ptr<video_frame::VideoFrame> receive_video_frame(int scaled_width, int scaled_height);

    [[nodiscard]] static std::unique_ptr<StreamInfo> find_best_stream(format_context::FormatContext* format_context, format_context::FormatContext::StreamType type);

private:
    format_context::FormatContext* format_context_ = nullptr;
    std::unique_ptr<codec_context::CodecContext> codec_context_;
    int stream_index_ = -1;

    std::string codec_type_;
    std::string codec_name_;
    std::string codec_additional_info_;
};

}  // namespace video_trimmer::video_content_provider::stream_info
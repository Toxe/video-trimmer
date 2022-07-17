#pragma once

#include <memory>

#include "../codec_context/codec_context.hpp"
#include "../format_context/format_context.hpp"
#include "../frame/frame.hpp"

namespace video_trimmer::video_reader::stream_info {

class StreamInfo {
public:
    StreamInfo(format_context::FormatContext* format_context, std::unique_ptr<codec_context::CodecContext> codec_context, int stream_index);

    [[nodiscard]] format_context::FormatContext* format_context() const { return format_context_; }
    [[nodiscard]] codec_context::CodecContext* codec_context() const { return codec_context_.get(); }
    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] double time_base() const;

    [[nodiscard]] std::unique_ptr<frame::Frame> receive_video_frame(int scaled_width, int scaled_height);
    [[nodiscard]] static std::unique_ptr<StreamInfo> find_best_stream(format_context::FormatContext* format_context, format_context::FormatContext::StreamType type);

    void set_dump_first_frame(bool dump_frame) { dump_first_frame_ = dump_frame; }

private:
    format_context::FormatContext* format_context_ = nullptr;
    std::unique_ptr<codec_context::CodecContext> codec_context_;
    int stream_index_ = -1;

    bool dump_first_frame_ = false;
};

}  // namespace video_trimmer::video_reader::stream_info

#pragma once

#include <memory>

#include "../codec_context/codec_context.hpp"
#include "../frame/frame.hpp"

namespace video_trimmer::video_reader::stream_info {

class StreamInfo {
public:
    enum class StreamType {
        audio,
        video
    };

    StreamInfo(std::unique_ptr<codec_context::CodecContext> codec_context, int stream_index);

    [[nodiscard]] codec_context::CodecContext* codec_context() const { return codec_context_.get(); }
    [[nodiscard]] int stream_index() const { return stream_index_; }

private:
    std::unique_ptr<codec_context::CodecContext> codec_context_;
    int stream_index_ = -1;
};

}  // namespace video_trimmer::video_reader::stream_info

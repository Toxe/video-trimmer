#include "stream_info.hpp"

StreamInfo::StreamInfo(FormatContext* format_context, std::unique_ptr<CodecContext> codec_context, int stream_index)
    : format_context_{format_context}, codec_context_{std::move(codec_context)}, stream_index_{stream_index}
{
    codec_type_ = codec_context_->codec_type();
    codec_name_ = codec_context_->codec_name();
    codec_additional_info_ = codec_context_->codec_additional_info();
}

double StreamInfo::time_base() const
{
    return format_context_->stream_time_base(stream_index_);
}

std::unique_ptr<VideoFrame> StreamInfo::receive_video_frame(const int scaled_width, const int scaled_height)
{
    std::unique_ptr<Frame> frame = codec_context_->receive_frame(time_base(), scaled_width, scaled_height);

    if (!frame)
        return nullptr;

    // copy decoded frame to image buffer
    frame->image_copy();

    return std::make_unique<VideoFrame>(std::move(frame));
}

std::unique_ptr<StreamInfo> StreamInfo::find_best_stream(FormatContext* format_context, const FormatContext::StreamType type)
{
    // find decoder for stream and allocate codec context for decoder
    AVStream* stream = format_context->find_best_stream(type);

    if (!stream)
        return nullptr;

    std::unique_ptr<CodecContext> codec_context = std::make_unique<CodecContext>(stream);

    if (!codec_context)
        return nullptr;

    return std::make_unique<StreamInfo>(format_context, std::move(codec_context), stream->index);
}

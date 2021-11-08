#include "stream_info.hpp"

#include "../adapters/format_context/format_context.hpp"
#include "../factory/factory.hpp"
#include "../video_frame/video_frame.hpp"

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

std::unique_ptr<VideoFrame> StreamInfo::receive_video_frame(Factory* factory, const int scaled_width, const int scaled_height)
{
    std::unique_ptr<Frame> frame = codec_context_->receive_frame(factory, time_base(), scaled_width, scaled_height);

    if (!frame)
        return nullptr;

    // copy decoded frame to image buffer
    frame->image_copy();

    return std::make_unique<VideoFrame>(std::move(frame));
}

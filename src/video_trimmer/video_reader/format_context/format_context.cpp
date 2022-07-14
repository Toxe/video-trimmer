#include "format_context.hpp"

#include <stdexcept>

extern "C" {
#include "libavutil/avutil.h"
#include "libavutil/rational.h"
}

namespace video_trimmer::video_reader::format_context {

FormatContext::FormatContext(const std::string_view& filename)
{
    // allocate format context
    format_context_ = AutoDeleteResource<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext* ctx) { avformat_close_input(&ctx); });

    if (!format_context_)
        throw std::runtime_error("avformat_alloc_context");

    // open input file
    int ret = avformat_open_input(format_context_.get_ptr(), filename.data(), nullptr, nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_open_input");

    // load stream info
    ret = avformat_find_stream_info(format_context_.get(), nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_find_stream_info");
}

double FormatContext::stream_time_base(const int stream_index) const
{
    const AVStream* stream = format_context_->streams[stream_index];
    return av_q2d(stream->time_base);
}

std::string FormatContext::format() const
{
    return format_context_->iformat->long_name;
}

AVStream* FormatContext::find_best_stream(FormatContext::StreamType type)
{
    const AVMediaType media_type = type == FormatContext::StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    // find decoder for stream
    return format_context_->streams[stream_index];
}

int FormatContext::read_frame(packet::Packet* packet)
{
    return av_read_frame(format_context_.get(), packet->packet());
}

}  // namespace video_trimmer::video_reader::format_context

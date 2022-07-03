#include "format_context.hpp"

#include <stdexcept>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/rational.h"
}

#include "threaded_stream_reader/packet/packet.hpp"

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

std::unique_ptr<StreamInfo> FormatContext::find_best_stream(StreamType type)
{
    const AVMediaType media_type = type == StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    // find decoder for stream
    AVStream* stream = format_context_->streams[stream_index];

    // allocate codec context for decoder
    std::unique_ptr<CodecContext> codec_context = std::make_unique<CodecContext>(stream);

    if (!codec_context)
        return nullptr;

    return std::make_unique<StreamInfo>(this, std::move(codec_context), stream_index);
}

int FormatContext::read_frame(Packet* packet)
{
    return av_read_frame(format_context_.get(), packet->packet());
}
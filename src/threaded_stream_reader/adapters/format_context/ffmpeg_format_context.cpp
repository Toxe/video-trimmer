#include "ffmpeg_format_context.hpp"

#include <stdexcept>

#include <fmt/core.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/rational.h>
}

#include "../../factory/factory.hpp"
#include "../codec_context/codec_context.hpp"
#include "../packet/packet.hpp"
#include "error/error.hpp"

FFmpegFormatContext::FFmpegFormatContext(const std::string_view& filename)
{
    // allocate format context
    format_context_ = auto_delete_ressource<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext* ctx) { avformat_close_input(&ctx); });

    if (!format_context_)
        throw std::runtime_error("avformat_alloc_context");

    // open input file
    AVFormatContext* p_ctx = format_context_.get();

    int ret = avformat_open_input(&p_ctx, filename.data(), nullptr, nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_open_input");

    // load stream info
    ret = avformat_find_stream_info(format_context_.get(), nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_find_stream_info");
}

double FFmpegFormatContext::stream_time_base(const int stream_index) const
{
    const AVStream* stream = format_context_->streams[stream_index];
    return av_q2d(stream->time_base);
}

const std::string FFmpegFormatContext::format() const
{
    return format_context_->iformat->long_name;
}

std::unique_ptr<StreamInfo> FFmpegFormatContext::find_best_stream(Factory* factory, const StreamType type)
{
    const AVMediaType media_type = type == StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0) {
        show_error(fmt::format("av_find_best_stream [{}]", av_get_media_type_string(media_type)), stream_index);
        return nullptr;
    }

    // find decoder for stream
    AVStream* stream = format_context_.get()->streams[stream_index];

    // allocate codec context for decoder
    std::unique_ptr<CodecContext> codec_context = factory->create_codec_context(stream);

    if (!codec_context)
        return nullptr;

    return std::make_unique<StreamInfo>(this, std::move(codec_context), stream_index);
}

int FFmpegFormatContext::read_frame(Packet* packet)
{
    return av_read_frame(format_context_.get(), packet->packet());
}

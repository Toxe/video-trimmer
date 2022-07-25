#include "codec_context.hpp"

#include <cassert>
#include <stdexcept>

#include "fmt/core.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixdesc.h"
}

#include "video_trimmer/error/error.hpp"

namespace video_trimmer::video_reader::codec_context {

CodecContext::CodecContext(AVStream* stream)
{
    assert(stream);

    const AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if (!decoder)
        throw std::runtime_error("avcodec_find_decoder");

    codec_context_ = AutoDeleteResource<AVCodecContext>(avcodec_alloc_context3(decoder), [](AVCodecContext* dec_ctx) { avcodec_free_context(&dec_ctx); });

    if (!codec_context_)
        throw std::runtime_error("avcodec_alloc_context3");

    // copy codec parameters from input stream to codec context
    int ret = avcodec_parameters_to_context(codec_context_.get(), stream->codecpar);

    if (ret < 0)
        throw std::runtime_error("avcodec_parameters_to_context");

    codec_context_->thread_count = 0;
    codec_context_->thread_type = FF_THREAD_FRAME;

    // init decoder
    ret = avcodec_open2(codec_context_.get(), decoder, nullptr);

    if (ret < 0)
        throw std::runtime_error("avcodec_open2");

    stream_index_ = stream->index;
    stream_time_base_ = av_q2d(stream->time_base);
    fps_ = codec_context_->codec_type == AVMEDIA_TYPE_VIDEO ? static_cast<float>(av_q2d(stream->avg_frame_rate)) : 0.0f;

    codec_type_ = av_get_media_type_string(codec_context_->codec_type);
    codec_name_ = codec_context_->codec->long_name;

    if (codec_context_->codec_type == AVMEDIA_TYPE_VIDEO)
        codec_additional_info_ = fmt::format("{}x{}, {:.1f} fps, {}", codec_context_->width, codec_context_->height, fps_, pixel_format_name());
    else if (codec_context_->codec_type == AVMEDIA_TYPE_AUDIO)
        codec_additional_info_ = fmt::format("{} channels, {} sample rate", codec_context_->channels, codec_context_->sample_rate);
}

Size CodecContext::size() const
{
    return {codec_context_->width, codec_context_->height};
}

AVPixelFormat CodecContext::pixel_format() const
{
    return codec_context_->pix_fmt;
}

std::string CodecContext::pixel_format_name() const
{
    return av_get_pix_fmt_name(codec_context_->pix_fmt);
}

int CodecContext::send_packet_to_decoder(AVPacket* packet)
{
    int ret = avcodec_send_packet(codec_context_.get(), packet);

    if (ret < 0)
        return video_trimmer::error::show_error("avcodec_send_packet", ret);

    return 0;
}

std::unique_ptr<frame::Frame> CodecContext::receive_frame_from_decoder(const double time_base)
{
    std::unique_ptr<frame::Frame> frame = std::make_unique<frame::Frame>(size(), pixel_format());

    int ret = avcodec_receive_frame(codec_context_.get(), frame->frame());

    if (ret < 0) {
        if (!(ret == AVERROR_EOF || ret == AVERROR(EAGAIN)))
            video_trimmer::error::show_error("avcodec_receive_frame");

        return nullptr;
    }

    frame->set_timestamp(static_cast<double>(frame->frame()->best_effort_timestamp) * time_base);

    return frame;
}

}  // namespace video_trimmer::video_reader::codec_context

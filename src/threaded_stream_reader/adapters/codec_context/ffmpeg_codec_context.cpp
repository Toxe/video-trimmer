#include "ffmpeg_codec_context.hpp"

#include <stdexcept>

#include <fmt/core.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
}

#include "../../../error/error.hpp"
#include "../../factory/factory.hpp"
#include "../frame/frame.hpp"
#include "../packet/packet.hpp"

FFmpegCodecContext::FFmpegCodecContext(AVStream* stream)
{
    const AVCodec* decoder = avcodec_find_decoder(stream->codecpar->codec_id);

    if (!decoder)
        throw std::runtime_error("avcodec_find_decoder");

    codec_context_ = auto_delete_ressource<AVCodecContext>(avcodec_alloc_context3(decoder), [](AVCodecContext* dec_ctx) { avcodec_free_context(&dec_ctx); });

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

    fps_ = codec_context_->codec_type == AVMEDIA_TYPE_VIDEO ? static_cast<float>(av_q2d(stream->avg_frame_rate)) : 0.0f;
}

std::string FFmpegCodecContext::codec_type()
{
    return av_get_media_type_string(codec_context_->codec_type);
}

std::string FFmpegCodecContext::codec_name()
{
    return codec_context_->codec->long_name;
}

std::string FFmpegCodecContext::codec_additional_info()
{
    std::string info;

    if (codec_context_->codec_type == AVMEDIA_TYPE_VIDEO)
        info = fmt::format("{}x{}, {:.1f} fps", codec_context_->width, codec_context_->height, fps_);
    else if (codec_context_->codec_type == AVMEDIA_TYPE_AUDIO)
        info = fmt::format("{} channels, {} sample rate", codec_context_->channels, codec_context_->sample_rate);

    return info;
}

int FFmpegCodecContext::width() const
{
    return codec_context_->width;
}

int FFmpegCodecContext::height() const
{
    return codec_context_->height;
}

AVPixelFormat FFmpegCodecContext::pixel_format() const
{
    return codec_context_->pix_fmt;
}

int FFmpegCodecContext::send_packet(Packet* packet)
{
    int ret = avcodec_send_packet(codec_context_.get(), packet->packet());

    if (ret < 0)
        return show_error("avcodec_send_packet", ret);

    return 0;
}

std::unique_ptr<Frame> FFmpegCodecContext::receive_frame(Factory* factory, const double time_base, const int scaled_width, const int scaled_height)
{
    std::unique_ptr<Frame> frame = factory->create_frame(this, scaled_width, scaled_height);

    int ret = avcodec_receive_frame(codec_context_.get(), frame->frame());

    if (ret < 0) {
        if (!(ret == AVERROR_EOF || ret == AVERROR(EAGAIN)))
            show_error("avcodec_receive_frame");

        return nullptr;
    }

    frame->set_timestamp(static_cast<double>(frame->frame()->best_effort_timestamp) * time_base);

    return frame;
}

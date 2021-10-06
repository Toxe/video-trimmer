#include "video_stream.hpp"

#include <spdlog/spdlog.h>

VideoStream::VideoStream(AVFormatContext* format_context, AVCodecContext* video_codec_context, int video_stream_index)
    : format_context_{format_context}, video_codec_context_{video_codec_context}, video_stream_index_{video_stream_index}
{
    spdlog::trace("init VideoStream");

    is_ready_ = init_stream();
}

int VideoStream::show_error(const std::string_view& error_message, std::optional<int> error_code)
{
    if (error_code.has_value()) {
        char buf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(error_code.value(), buf, AV_ERROR_MAX_STRING_SIZE);
        spdlog::error("error: {} ({})\n", error_message, buf);
        return error_code.value();
    } else {
        spdlog::error("error: {}\n", error_message);
        return -1;
    }
}

int VideoStream::resize_scaling_context(AVCodecContext* codec_context, int width, int height)
{
    spdlog::debug("resize scaling context to {}x{}", width, height);

    scale_width_ = width;
    scale_height_ = height;

    // TODO
    // av_free(dst_buf_size_[0]);
    // av_free(img_buf_size_[0]);

    dst_buf_size_ = av_image_alloc(dst_buf_data_.data(), dst_buf_linesize_.data(), scale_width_, scale_height_, AV_PIX_FMT_RGBA, 1);

    if (img_buf_size_ < 0)
        return show_error("av_image_alloc", dst_buf_size_);

    scaling_context_.reset(sws_getContext(codec_context->width, codec_context->height, codec_context->pix_fmt, scale_width_, scale_height_, AV_PIX_FMT_RGBA, SWS_BILINEAR, nullptr, nullptr, nullptr));

    if (!scaling_context_)
        return show_error("sws_getContext");

    return 0;
}

int VideoStream::init_stream()
{
    // allocate buffer for decoded source images
    img_buf_size_ = av_image_alloc(img_buf_data_.data(), img_buf_linesize_.data(), video_codec_context_->width, video_codec_context_->height, video_codec_context_->pix_fmt, 1);

    if (img_buf_size_ < 0)
        return show_error("av_image_alloc", img_buf_size_);

    // allocate buffer for scaled output images
    dst_buf_size_ = av_image_alloc(dst_buf_data_.data(), dst_buf_linesize_.data(), video_codec_context_->width, video_codec_context_->height, AV_PIX_FMT_RGBA, 1);

    if (img_buf_size_ < 0)
        return show_error("av_image_alloc", dst_buf_size_);

    // create scaling context
    scale_width_ = video_codec_context_->width;
    scale_height_ = video_codec_context_->height;

    scaling_context_ = auto_delete_ressource<SwsContext>(sws_getContext(video_codec_context_->width, video_codec_context_->height, video_codec_context_->pix_fmt, scale_width_, scale_height_, AV_PIX_FMT_RGBA, SWS_BILINEAR, nullptr, nullptr, nullptr), [](SwsContext* ctx) { sws_freeContext(ctx); });

    if (!scaling_context_)
        return show_error("sws_getContext");

    frame_ = auto_delete_ressource<AVFrame>(av_frame_alloc(), [](AVFrame* f) { av_frame_free(&f); });

    if (!frame_)
        return show_error("av_frame_alloc");

    packet_ = auto_delete_ressource<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });

    if (!packet_)
        return show_error("av_packet_alloc");

    // TODO
    // av_free(dst_buf_size_[0]);
    // av_free(img_buf_size_[0]);

    return 0;
}

bool VideoStream::next_frame(sf::Texture& texture)
{
    while (true) {
        int ret = av_read_frame(format_context_, packet_.get());

        if (ret < 0)
            return false;

        // process only interesting packets, skip the rest
        if (packet_->stream_index == video_stream_index_) {
            ret = decode_packet(video_codec_context_, packet_.get(), frame_.get(), texture);
            av_packet_unref(packet_.get());
            return ret == 0;
        }

        av_packet_unref(packet_.get());
    }

    return true;
}

int VideoStream::decode_packet(AVCodecContext* codec_context, const AVPacket* packet, AVFrame* frame, sf::Texture& texture)
{
    // send packet to the decoder
    int ret = avcodec_send_packet(codec_context, packet);

    if (ret < 0)
        return show_error("avcodec_send_packet", ret);

    // get all available frames from the decoder
    while (ret >= 0) {
        ret = avcodec_receive_frame(codec_context, frame);

        if (ret < 0) {
            if (ret == AVERROR_EOF || ret == AVERROR(EAGAIN))
                return 0;

            return show_error("avcodec_receive_frame", ret);
        }

        // copy decoded frame to image buffer
        av_image_copy(img_buf_data_.data(), img_buf_linesize_.data(), const_cast<const uint8_t**>(frame->data), frame->linesize, codec_context->pix_fmt, codec_context->width, codec_context->height);

        // convert to destination format
        if (scale_width_ != static_cast<int>(texture.getSize().x) || scale_height_ != static_cast<int>(texture.getSize().y))
            resize_scaling_context(codec_context, texture.getSize().x, texture.getSize().y);

        if (scaling_context_)
            sws_scale(scaling_context_.get(), img_buf_data_.data(), img_buf_linesize_.data(), 0, codec_context->height, dst_buf_data_.data(), dst_buf_linesize_.data());

        texture.update(dst_buf_data_[0]);

        av_frame_unref(frame);

        if (ret < 0)
            return ret;
    }

    return 0;
}

#include "ffmpeg_frame.hpp"

#include <stdexcept>

extern "C" {
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/mem.h>
}

#include "../codec_context/codec_context.hpp"

FFmpegFrame::FFmpegFrame(CodecContext* codec_context, const int scaled_width, const int scaled_height)
    : Frame(codec_context, scaled_width, scaled_height)
{
    src_pixel_format_ = codec_context->pixel_format();

    frame_ = auto_delete_ressource<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });

    if (!frame_)
        throw std::runtime_error("av_frame_alloc");

    // allocate buffer for decoded source images
    int buf_size = av_image_alloc(img_buf_data_.data(), img_buf_linesize_.data(), src_width(), src_height(), src_pixel_format_, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");

    // allocate buffer for scaled output images
    buf_size = av_image_alloc(dst_buf_data_.data(), dst_buf_linesize_.data(), dst_width(), dst_height(), AV_PIX_FMT_RGBA, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");
}

FFmpegFrame::~FFmpegFrame()
{
    av_freep(dst_buf_data_.data());
    av_freep(img_buf_data_.data());
}

std::span<const uint8_t> FFmpegFrame::pixels()
{
    return std::span<const uint8_t>{dst_buf_data_[0], static_cast<std::size_t>(4 * dst_width() * dst_height())};
}

void FFmpegFrame::image_copy()
{
    av_image_copy(src_data(), src_linesizes(), const_cast<const uint8_t**>(frame_->data), &frame_->linesize[0], src_pixel_format_, src_width(), src_height());
    av_frame_unref(frame_.get());
}

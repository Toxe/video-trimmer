#pragma once

#include "frame.hpp"

#include <array>

#include "auto_delete_resource.hpp"

extern "C" {
#include <libavutil/pixfmt.h>
}

struct AVFrame;

class FFmpegFrame : public Frame {
    auto_delete_resource<AVFrame> frame_ = {nullptr, nullptr};

    std::array<uint8_t*, 4> img_buf_data_ = {nullptr};
    std::array<uint8_t*, 4> dst_buf_data_ = {nullptr};
    std::array<int, 4> img_buf_linesize_ = {0};
    std::array<int, 4> dst_buf_linesize_ = {0};

    AVPixelFormat src_pixel_format_;

public:
    FFmpegFrame(CodecContext* codec_context, int scaled_width, int scaled_height);
    ~FFmpegFrame() override;

    [[nodiscard]] AVFrame* frame() override { return frame_.get(); }

    [[nodiscard]] uint8_t** src_data() override { return img_buf_data_.data(); }
    [[nodiscard]] uint8_t** dst_data() override { return dst_buf_data_.data(); }
    [[nodiscard]] int* src_linesizes() override { return img_buf_linesize_.data(); }
    [[nodiscard]] int* dst_linesizes() override { return dst_buf_linesize_.data(); }

    [[nodiscard]] std::span<const uint8_t> pixels() override;

    void image_copy() override;
};

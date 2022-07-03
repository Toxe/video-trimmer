#pragma once

#include <array>
#include <cstdint>
#include <span>

#include "auto_delete_resource.hpp"

extern "C" {
#include "libavutil/pixfmt.h"
}

struct AVFrame;

class CodecContext;

class Frame {
public:
    Frame(CodecContext* codec_context, int scaled_width, int scaled_height);
    ~Frame();

    [[nodiscard]] double timestamp() const { return timestamp_; };
    void set_timestamp(double timestamp) { timestamp_ = timestamp; };

    [[nodiscard]] int src_width() const { return src_width_; }
    [[nodiscard]] int src_height() const { return src_height_; }
    [[nodiscard]] int dst_width() const { return dst_width_; }
    [[nodiscard]] int dst_height() const { return dst_height_; }

    [[nodiscard]] AVFrame* frame() { return frame_.get(); }

    [[nodiscard]] uint8_t** src_data() { return img_buf_data_.data(); }
    [[nodiscard]] uint8_t** dst_data() { return dst_buf_data_.data(); }
    [[nodiscard]] int* src_linesizes() { return img_buf_linesize_.data(); }
    [[nodiscard]] int* dst_linesizes() { return dst_buf_linesize_.data(); }

    [[nodiscard]] std::span<const uint8_t> pixels();

    void image_copy();

private:
    int src_width_, src_height_;
    int dst_width_, dst_height_;

    double timestamp_ = 0.0;

    AutoDeleteResource<AVFrame> frame_;

    std::array<uint8_t*, 4> img_buf_data_ = {nullptr};
    std::array<uint8_t*, 4> dst_buf_data_ = {nullptr};
    std::array<int, 4> img_buf_linesize_ = {0};
    std::array<int, 4> dst_buf_linesize_ = {0};

    AVPixelFormat src_pixel_format_;
};

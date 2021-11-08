#pragma once

#include <cstdint>
#include <span>

struct AVFrame;

class CodecContext;

class Frame {
    int src_width_, src_height_;
    int dst_width_, dst_height_;

    double timestamp_ = 0.0;

public:
    Frame(CodecContext* codec_context, const int scaled_width, const int scaled_height);
    virtual ~Frame() = default;

    [[nodiscard]] double timestamp() const;
    void set_timestamp(const double timestamp);

    [[nodiscard]] int src_width() const { return src_width_; }
    [[nodiscard]] int src_height() const { return src_height_; }
    [[nodiscard]] int dst_width() const { return dst_width_; }
    [[nodiscard]] int dst_height() const { return dst_height_; }

    [[nodiscard]] virtual AVFrame* frame() = 0;

    [[nodiscard]] virtual uint8_t** src_data() = 0;
    [[nodiscard]] virtual uint8_t** dst_data() = 0;
    [[nodiscard]] virtual int* src_linesizes() = 0;
    [[nodiscard]] virtual int* dst_linesizes() = 0;

    [[nodiscard]] virtual std::span<const uint8_t> pixels() = 0;

    virtual void image_copy() = 0;
};

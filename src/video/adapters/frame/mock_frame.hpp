#pragma once

#include "frame.hpp"

class MockFrame : public Frame {
public:
    MockFrame(CodecContext* codec_context, const int scaled_width, const int scaled_height);

    [[nodiscard]] virtual AVFrame* frame() override { return nullptr; }

    [[nodiscard]] virtual uint8_t** src_data() override { return nullptr; }
    [[nodiscard]] virtual uint8_t** dst_data() override { return nullptr; }
    [[nodiscard]] virtual int* src_linesizes() override { return nullptr; }
    [[nodiscard]] virtual int* dst_linesizes() override { return nullptr; }

    [[nodiscard]] std::span<const uint8_t> pixels() override;

    virtual void image_copy() override;
};

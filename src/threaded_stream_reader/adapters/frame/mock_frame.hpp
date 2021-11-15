#pragma once

#include "frame.hpp"

class MockFrame : public Frame {
public:
    MockFrame(CodecContext* codec_context, int scaled_width, int scaled_height);

    [[nodiscard]] AVFrame* frame() override { return nullptr; }

    [[nodiscard]] uint8_t** src_data() override { return nullptr; }
    [[nodiscard]] uint8_t** dst_data() override { return nullptr; }
    [[nodiscard]] int* src_linesizes() override { return nullptr; }
    [[nodiscard]] int* dst_linesizes() override { return nullptr; }

    [[nodiscard]] std::span<const uint8_t> pixels() override;

    void image_copy() override;
};

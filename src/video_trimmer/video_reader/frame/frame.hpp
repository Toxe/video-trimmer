#pragma once

#include <memory>
#include <span>
#include <string>

extern "C" {
#include "libavutil/pixfmt.h"
}

struct AVFrame;

namespace video_trimmer::video_reader::frame {

class Frame {
public:
    Frame(int width, int height, int scaled_width, int scaled_height, AVPixelFormat pixel_format);
    ~Frame();

    [[nodiscard]] bool is_audio_frame() const;
    [[nodiscard]] bool is_video_frame() const;

    [[nodiscard]] double timestamp() const;
    void set_timestamp(double timestamp);

    [[nodiscard]] int src_width() const;
    [[nodiscard]] int src_height() const;
    [[nodiscard]] int dst_width() const;
    [[nodiscard]] int dst_height() const;

    [[nodiscard]] AVFrame* frame();

    [[nodiscard]] uint8_t** src_data();
    [[nodiscard]] uint8_t** dst_data();
    [[nodiscard]] int* src_linesizes();
    [[nodiscard]] int* dst_linesizes();

    [[nodiscard]] std::span<const uint8_t> pixels();

    void image_copy();

    void dump_to_file(const std::string& filename);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_reader::frame

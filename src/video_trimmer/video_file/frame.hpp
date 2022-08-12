#pragma once

#include <memory>
#include <span>
#include <string>

#include "pixel_format.hpp"
#include "types.hpp"

struct AVFrame;

namespace video_trimmer::video_file {

class Frame {
public:
    Frame(Size size, PixelFormat pixel_format);
    ~Frame();

    [[nodiscard]] bool is_audio_frame() const;
    [[nodiscard]] bool is_video_frame() const;

    [[nodiscard]] Size size() const;

    [[nodiscard]] double timestamp() const;
    void set_timestamp(double timestamp);

    [[nodiscard]] uint8_t** data();
    [[nodiscard]] int* linesizes();

    [[nodiscard]] AVFrame* frame();
    [[nodiscard]] PixelFormat pixel_format() const;

    [[nodiscard]] std::string info() const;

    void dump_to_file(const std::string& filename);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_file

#pragma once

#include <chrono>
#include <memory>
#include <span>
#include <string>

#include "pixel_format.hpp"
#include "types.hpp"

struct AVFrame;

namespace video_trimmer::video_file {

class Frame {
private:
    class Impl;

public:
    explicit Frame(std::unique_ptr<Impl> impl);
    ~Frame();

    [[nodiscard]] static std::unique_ptr<Frame> create_audio_frame();
    [[nodiscard]] static std::unique_ptr<Frame> create_video_frame();
    [[nodiscard]] static std::unique_ptr<Frame> create_video_frame(Size size, PixelFormat pixel_format, int64_t stream_duration);
    [[nodiscard]] static std::unique_ptr<Frame> create_video_frame(Size size, PixelFormat pixel_format, int64_t stream_duration, std::chrono::microseconds timestamp, std::chrono::microseconds duration, char picture_type);

    void update_from_frame(double stream_time_base);

    [[nodiscard]] bool is_audio_frame() const;
    [[nodiscard]] bool is_video_frame() const;

    [[nodiscard]] Size size() const;

    [[nodiscard]] std::chrono::microseconds timestamp() const;
    [[nodiscard]] std::chrono::microseconds duration() const;

    [[nodiscard]] uint8_t** data();
    [[nodiscard]] int* linesizes();

    [[nodiscard]] AVFrame* frame();
    [[nodiscard]] PixelFormat pixel_format() const;

    [[nodiscard]] std::string description() const;

    void dump_to_file(const std::string& filename);

private:
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_file

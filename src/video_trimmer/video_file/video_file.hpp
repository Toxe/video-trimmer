#pragma once

#include <memory>
#include <string>

#include "codec_context.hpp"
#include "frame.hpp"

namespace video_trimmer::video_file {

class VideoFile {
public:
    explicit VideoFile(const std::string& full_filename);
    ~VideoFile();

    [[nodiscard]] bool is_open() const;
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const;
    [[nodiscard]] const std::string& file_format() const;
    [[nodiscard]] std::string format_duration() const;

    [[nodiscard]] CodecContext* audio_codec_context() const;
    [[nodiscard]] CodecContext* video_codec_context() const;

    [[nodiscard]] bool has_audio_stream() const;
    [[nodiscard]] bool has_video_stream() const;

    [[nodiscard]] std::unique_ptr<Frame> read_next_frame(double playback_position);

    [[nodiscard]] bool is_supported_pixel_format() const;

    void set_dump_first_frame(bool dump_frame);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_file

#pragma once

#include <memory>
#include <string>

#include "../codec_context/codec_context.hpp"
#include "../frame/frame.hpp"

namespace video_trimmer::video_reader::video_file {

class VideoFile {
public:
    explicit VideoFile(const std::string& full_filename);
    ~VideoFile();

    [[nodiscard]] bool is_open() const;
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const;
    [[nodiscard]] const std::string& file_format() const;

    [[nodiscard]] codec_context::CodecContext* audio_codec_context() const;
    [[nodiscard]] codec_context::CodecContext* video_codec_context() const;

    [[nodiscard]] bool has_audio_stream() const;
    [[nodiscard]] bool has_video_stream() const;

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position);

    void set_dump_first_frame(bool dump_frame);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_reader::video_file

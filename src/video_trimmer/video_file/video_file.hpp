#pragma once

#include <memory>
#include <string>

#include "../clock/video_duration.hpp"
#include "codec_context.hpp"
#include "frame.hpp"

namespace video_trimmer::video_file {

class VideoFile {
public:
    explicit VideoFile(const std::string& full_filename);
    virtual ~VideoFile();

    [[nodiscard]] virtual bool is_open() const;
    [[nodiscard]] virtual bool is_video() const;

    [[nodiscard]] virtual const std::string& filename() const;
    [[nodiscard]] virtual const std::string& file_format() const;

    [[nodiscard]] virtual clock::VideoDuration duration() const;
    [[nodiscard]] virtual std::string format_duration() const;

    [[nodiscard]] virtual CodecContext* audio_codec_context() const;
    [[nodiscard]] virtual CodecContext* video_codec_context() const;

    [[nodiscard]] virtual bool has_audio_stream() const;
    [[nodiscard]] virtual bool has_video_stream() const;

    [[nodiscard]] virtual std::unique_ptr<Frame> read_next_frame();

    [[nodiscard]] virtual bool seek_position(clock::PlaybackPosition position);

    virtual void set_dump_first_frame(bool dump_frame);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_file

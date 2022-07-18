#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../format_context/format_context.hpp"
#include "../stream_info/stream_info.hpp"

namespace video_trimmer::video_reader::video_file {

class VideoFile {
public:
    explicit VideoFile(const std::string_view& full_filename);
    ~VideoFile();

    [[nodiscard]] bool is_open() const;
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const;
    [[nodiscard]] const std::string& file_format() const;

    [[nodiscard]] stream_info::StreamInfo* audio_stream_info() const;
    [[nodiscard]] stream_info::StreamInfo* video_stream_info() const;

    [[nodiscard]] bool has_audio_stream() const;
    [[nodiscard]] bool has_video_stream() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_reader::video_file

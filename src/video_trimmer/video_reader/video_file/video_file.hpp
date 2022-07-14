#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../format_context/format_context.hpp"
#include "../stream_info/stream_info.hpp"

namespace video_trimmer::video_reader::video_file {

class VideoFile {
public:
    VideoFile(const std::string_view& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }

    [[nodiscard]] stream_info::StreamInfo* audio_stream_info() const { return audio_stream_info_.get(); }
    [[nodiscard]] stream_info::StreamInfo* video_stream_info() const { return video_stream_info_.get(); }

    [[nodiscard]] bool has_audio_stream() const;
    [[nodiscard]] bool has_video_stream() const;

private:
    std::unique_ptr<format_context::FormatContext> format_context_;
    std::unique_ptr<stream_info::StreamInfo> audio_stream_info_;
    std::unique_ptr<stream_info::StreamInfo> video_stream_info_;

    bool is_open_ = false;

    std::string filename_without_path_;
    std::string file_format_;

    int open_file(const std::string_view& full_filename);
};

}  // namespace video_trimmer::video_reader::video_file

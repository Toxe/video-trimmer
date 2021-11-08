#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "adapters/format_context/format_context.hpp"
#include "stream_info/stream_info.hpp"

class Factory;

class VideoFile {
    Factory* factory_;

    std::unique_ptr<FormatContext> format_context_;
    std::unique_ptr<StreamInfo> audio_stream_info_;
    std::unique_ptr<StreamInfo> video_stream_info_;

    bool is_open_ = false;

    std::string filename_without_path_;
    std::string file_format_;

    int open_file(const std::string_view& full_filename);

public:
    VideoFile(const std::string_view& full_filename, Factory* factory);

    [[nodiscard]] bool is_open() const { return is_open_; }

    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }

    [[nodiscard]] StreamInfo* audio_stream_info() const { return audio_stream_info_.get(); }
    [[nodiscard]] StreamInfo* video_stream_info() const { return video_stream_info_.get(); }
};

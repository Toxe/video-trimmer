#pragma once

#include <string>
#include <vector>

#include "stream_info.hpp"

class VideoFile {
    bool is_open_ = false;

    std::string filename_;
    std::string file_format_;

    std::vector<StreamInfo> streams_;

    bool open_file(const std::string& full_filename);

public:
    VideoFile(const std::string& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] const std::string& filename() const { return filename_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }
    [[nodiscard]] int number_of_streams() const { return static_cast<int>(streams_.size()); }
    [[nodiscard]] const StreamInfo& stream_info(const int stream_id) const { return streams_[stream_id]; }
};

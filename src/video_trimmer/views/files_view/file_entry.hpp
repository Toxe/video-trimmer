#pragma once

#include <filesystem>
#include <string>

namespace video_trimmer::video_reader::video_file {
class VideoFile;
}  // namespace video_trimmer::video_reader::video_file

namespace video_trimmer::views::files_view {

class FileEntry {
public:
    FileEntry(const video_reader::video_file::VideoFile& video_file, std::filesystem::path path);

    [[nodiscard]] const std::string& basename() const { return basename_; }
    [[nodiscard]] const std::string& full_filename() const { return full_filename_; }

    [[nodiscard]] const std::string& video_duration() const { return video_duration_; }
    [[nodiscard]] const std::string& video_format() const { return video_format_; }
    [[nodiscard]] const std::string& video_codec_name() const { return video_codec_name_; }
    [[nodiscard]] const std::string& video_codec_additional_info() const { return video_codec_additional_info_; }

    [[nodiscard]] const std::string& file_size() const { return file_size_; }

private:
    std::filesystem::path path_;

    std::string basename_;
    std::string full_filename_;

    std::string video_duration_;
    std::string video_format_;
    std::string video_codec_name_;
    std::string video_codec_additional_info_;

    std::string file_size_;
};

}  // namespace video_trimmer::views::files_view

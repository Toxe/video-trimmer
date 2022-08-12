#include "file_entry.hpp"

#include <cmath>
#include <utility>

#include "fmt/core.h"

#include "../../video_file/video_file.hpp"

namespace video_trimmer::views::files_view {

FileEntry::FileEntry(const video_file::VideoFile& video_file, std::filesystem::path path)
    : path_(std::move(path))
{
    basename_ = path_.filename().string();
    full_filename_ = path_.string();

    video_duration_ = video_file.format_duration();
    video_format_ = video_file.file_format();
    video_codec_name_ = video_file.video_codec_context()->codec_name();
    video_codec_additional_info_ = video_file.video_codec_context()->codec_additional_info();

    const auto filesize = std::filesystem::file_size(path_);

    if (filesize < 1024)
        file_size_ = "<1 KB";
    else if (filesize < (1024 * 1024))
        file_size_ = fmt::format("{} KB", std::round(static_cast<double>(filesize) / 1024.0));
    else
        file_size_ = fmt::format("{} MB", std::round(static_cast<double>(filesize) / (1024.0 * 1024.0)));

    is_supported_ = video_file.video_codec_context()->pixel_format().is_supported();

    if (!is_supported_)
        not_supported_note_ = fmt::format("Pixel format {} is not supported.", video_file.video_codec_context()->pixel_format().name());
}

}  // namespace video_trimmer::views::files_view

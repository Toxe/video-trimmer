#include "video_file.hpp"

#include <filesystem>

#include "error/error.hpp"
#include "factory/factory.hpp"

VideoFile::VideoFile(const std::string_view& full_filename, Factory* factory)
    : factory_{factory}
{
    is_open_ = open_file(full_filename) == 0;
}

int VideoFile::open_file(const std::string_view& full_filename)
{
    std::filesystem::path path{full_filename};

    if (!std::filesystem::exists(path))
        return show_error("file not found");

    filename_without_path_ = path.filename().string();

    // allocate format context
    format_context_ = factory_->create_format_context(full_filename);

    if (!format_context_)
        return -1;

    file_format_ = format_context_->format();

    // find best audio and video stream
    audio_stream_info_ = format_context_->find_best_stream(factory_, FormatContext::StreamType::audio);
    video_stream_info_ = format_context_->find_best_stream(factory_, FormatContext::StreamType::video);

    if (!audio_stream_info_ || !video_stream_info_)
        return show_error("unable to find streams");

    return 0;
}

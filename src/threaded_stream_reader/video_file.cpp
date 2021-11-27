#include "video_file.hpp"

#include <filesystem>
#include <stdexcept>

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

    try {
        // allocate format context
        format_context_ = factory_->create_format_context(full_filename);

        if (!format_context_)
            return -1;

        file_format_ = format_context_->format();

        // find best audio and video stream
        audio_stream_info_ = format_context_->find_best_stream(factory_, FormatContext::StreamType::audio);
        video_stream_info_ = format_context_->find_best_stream(factory_, FormatContext::StreamType::video);

        // a missing audio stream is fine, but we are looking for at least a video stream
        if (!video_stream_info_)
            return -1;
    } catch (const std::runtime_error&) {
        return -1;
    }

    return 0;
}

bool VideoFile::is_video() const
{
    return is_open() && has_video_stream() && video_stream_info_->codec_context()->fps() > 0.0f;
}

bool VideoFile::has_audio_stream() const
{
    return audio_stream_info_ != nullptr;
}

bool VideoFile::has_video_stream() const
{
    return video_stream_info_ != nullptr;
}

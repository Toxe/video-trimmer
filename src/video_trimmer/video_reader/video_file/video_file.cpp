#include "video_file.hpp"

#include <filesystem>
#include <stdexcept>

#include "video_trimmer/error/error.hpp"

namespace video_trimmer::video_reader::video_file {

class VideoFile::Impl {
public:
    explicit Impl(const std::string_view& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }

    [[nodiscard]] stream_info::StreamInfo* audio_stream_info() const { return audio_stream_info_.get(); }
    [[nodiscard]] stream_info::StreamInfo* video_stream_info() const { return video_stream_info_.get(); }

    [[nodiscard]] bool has_audio_stream() const { return audio_stream_info_ != nullptr; };
    [[nodiscard]] bool has_video_stream() const { return video_stream_info_ != nullptr; };

private:
    std::unique_ptr<format_context::FormatContext> format_context_;
    std::unique_ptr<stream_info::StreamInfo> audio_stream_info_;
    std::unique_ptr<stream_info::StreamInfo> video_stream_info_;

    std::string filename_without_path_;
    std::string file_format_;

    bool is_open_ = false;

    int open_file(const std::string_view& full_filename);
};

VideoFile::Impl::Impl(const std::string_view& full_filename)
{
    is_open_ = open_file(full_filename) == 0;
}

int VideoFile::Impl::open_file(const std::string_view& full_filename)
{
    std::filesystem::path path{full_filename};

    if (!std::filesystem::exists(path))
        return video_trimmer::error::show_error("file not found");

    filename_without_path_ = path.filename().string();

    try {
        // allocate format context
        format_context_ = std::make_unique<format_context::FormatContext>(full_filename);

        if (!format_context_)
            return -1;

        file_format_ = format_context_->format();

        // find best audio and video stream
        audio_stream_info_ = stream_info::StreamInfo::find_best_stream(format_context_.get(), format_context::FormatContext::StreamType::audio);
        video_stream_info_ = stream_info::StreamInfo::find_best_stream(format_context_.get(), format_context::FormatContext::StreamType::video);

        // a missing audio stream is fine, but we are looking for at least a video stream
        if (!video_stream_info_)
            return -1;
    } catch (const std::runtime_error&) {
        return -1;
    }

    return 0;
}

bool VideoFile::Impl::is_video() const
{
    return is_open() && has_video_stream() && video_stream_info_->codec_context()->fps() > 0.0f;
}

VideoFile::VideoFile(const std::string_view& full_filename) : impl_(std::make_unique<VideoFile::Impl>(full_filename)) { }
VideoFile::~VideoFile() = default;
bool VideoFile::is_open() const { return impl_->is_open(); }
bool VideoFile::is_video() const { return impl_->is_video(); }
const std::string& VideoFile::filename() const { return impl_->filename(); }
const std::string& VideoFile::file_format() const { return impl_->file_format(); }
stream_info::StreamInfo* VideoFile::audio_stream_info() const { return impl_->audio_stream_info(); }
stream_info::StreamInfo* VideoFile::video_stream_info() const { return impl_->video_stream_info(); }
bool VideoFile::has_audio_stream() const { return impl_->has_audio_stream(); }
bool VideoFile::has_video_stream() const { return impl_->has_video_stream(); }

}  // namespace video_trimmer::video_reader::video_file

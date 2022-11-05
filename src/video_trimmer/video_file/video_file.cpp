#include "video_file.hpp"

#include <cassert>
#include <filesystem>
#include <stdexcept>

#include "fmt/core.h"

extern "C" {
#include "libavcodec/packet.h"
#include "libavformat/avformat.h"
#include "libavutil/rational.h"
}

#include "../error/error.hpp"
#include "auto_delete_resource.hpp"

namespace video_trimmer::video_file {

class VideoFile::Impl {
public:
    explicit Impl(const std::string& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }

    [[nodiscard]] clock::VideoDuration duration() const;
    [[nodiscard]] std::string format_duration() const;

    [[nodiscard]] CodecContext* audio_codec_context() const { return audio_codec_context_.get(); }
    [[nodiscard]] CodecContext* video_codec_context() const { return video_codec_context_.get(); }

    [[nodiscard]] bool has_audio_stream() const { return audio_codec_context_ != nullptr; };
    [[nodiscard]] bool has_video_stream() const { return video_codec_context_ != nullptr; };

    [[nodiscard]] std::unique_ptr<Frame> read_next_frame();

    [[nodiscard]] bool seek_position(clock::PlaybackPosition position);

    void set_dump_first_frame(bool dump_frame) { dump_first_frame_ = dump_frame; }

private:
    AutoDeleteResource<AVFormatContext> format_context_;
    AutoDeleteResource<AVPacket> packet_;

    std::unique_ptr<CodecContext> audio_codec_context_;
    std::unique_ptr<CodecContext> video_codec_context_;

    std::string filename_without_path_;
    std::string file_format_;

    bool is_open_ = false;
    bool dump_first_frame_ = false;

    [[nodiscard]] int open_file(const std::string& full_filename);

    [[nodiscard]] std::unique_ptr<CodecContext> find_best_stream(CodecContext::StreamType type);
    [[nodiscard]] std::unique_ptr<Frame> decode_video_packet(AVPacket* packet);
};

VideoFile::Impl::Impl(const std::string& full_filename)
{
    av_log_set_level(AV_LOG_FATAL);

    packet_ = AutoDeleteResource<AVPacket>(av_packet_alloc(), [](AVPacket* p) { av_packet_free(&p); });

    if (!packet_)
        throw std::runtime_error("av_packet_alloc");

    is_open_ = open_file(full_filename) == 0;
}

int VideoFile::Impl::open_file(const std::string& full_filename)
{
    std::filesystem::path path{full_filename};

    if (!std::filesystem::exists(path))
        return error::show_error("file not found");

    filename_without_path_ = path.filename().string();

    // allocate format context
    format_context_ = AutoDeleteResource<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext* ctx) { avformat_close_input(&ctx); });

    if (!format_context_)
        return error::show_error("avformat_alloc_context");

    // open input file
    int ret = avformat_open_input(format_context_.get_ptr(), full_filename.data(), nullptr, nullptr);

    if (ret < 0)
        return ret;

    // load stream info
    ret = avformat_find_stream_info(format_context_.get(), nullptr);

    if (ret < 0)
        return error::show_error("avformat_find_stream_info", ret);

    file_format_ = format_context_->iformat->long_name;

    // find best audio and video stream
    audio_codec_context_ = find_best_stream(CodecContext::StreamType::audio);
    video_codec_context_ = find_best_stream(CodecContext::StreamType::video);

    // a missing audio stream is fine, but we are looking for at least a video stream
    if (!video_codec_context_)
        return -1;

    return 0;
}

bool VideoFile::Impl::is_video() const
{
    return is_open() && has_video_stream() && video_codec_context_->fps() > 0.0f;
}

std::unique_ptr<CodecContext> VideoFile::Impl::find_best_stream(CodecContext::StreamType type)
{
    // find decoder for stream and allocate codec context for decoder
    const AVMediaType media_type = type == CodecContext::StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    AVStream* stream = format_context_->streams[stream_index];

    if (!stream)
        return nullptr;

    try {
        return std::make_unique<CodecContext>(stream);
    } catch (const std::exception&) {
        return nullptr;
    }
}

std::unique_ptr<Frame> VideoFile::Impl::read_next_frame()
{
    // read until we get at least one video frame
    while (true) {
        if (av_read_frame(format_context_.get(), packet_.get()) < 0)
            break;

        // process only interesting packets, drop the rest
        if (packet_->stream_index == video_codec_context_->stream_index()) {
            std::unique_ptr<Frame> frame = decode_video_packet(packet_.get());
            av_packet_unref(packet_.get());
            return frame;
        } else if (packet_->stream_index == audio_codec_context_->stream_index()) {
            // TODO: decode audio packet
            av_packet_unref(packet_.get());
        } else {
            av_packet_unref(packet_.get());
        }
    }

    return nullptr;
}

std::unique_ptr<Frame> VideoFile::Impl::decode_video_packet(AVPacket* packet)
{
    // send packet to the decoder
    if (video_codec_context_->send_packet_to_decoder(packet) < 0)
        return nullptr;

    // get available frame from the decoder
    std::unique_ptr<Frame> frame = video_codec_context_->receive_frame_from_decoder();

    if (dump_first_frame_ && frame) {
        frame->dump_to_file(filename_without_path_);
        dump_first_frame_ = false;
    }

    return frame;
}

clock::VideoDuration VideoFile::Impl::duration() const
{
    static_assert(AV_TIME_BASE == 1'000'000);

    if (format_context_->duration == AV_NOPTS_VALUE)
        return clock::VideoDuration{};

    return clock::VideoDuration{std::chrono::microseconds{format_context_->duration}};
}

std::string VideoFile::Impl::format_duration() const
{
    // based on av_dump_format (https://ffmpeg.org/doxygen/trunk/dump_8c_source.html#l00630)
    if (format_context_->duration == AV_NOPTS_VALUE)
        return "N/A";

    int64_t secs = format_context_->duration / AV_TIME_BASE;
    int64_t mins = secs / 60;
    secs %= 60;
    int64_t hours = mins / 60;
    mins %= 60;

    return fmt::format("{:02}:{:02}:{:02}", hours, mins, secs);
}

bool VideoFile::Impl::seek_position(clock::PlaybackPosition position)
{
    static_assert(AV_TIME_BASE == 1'000'000);

    const int ret = av_seek_frame(format_context_.get(), -1, position.microseconds().count(), AVSEEK_FLAG_BACKWARD);

    if (ret < 0) {
        error::show_error("av_seek_frame", ret);
        return false;
    }

    if (audio_codec_context_)
        audio_codec_context_->flush_buffers();

    if (video_codec_context_)
        video_codec_context_->flush_buffers();

    return true;
}

VideoFile::VideoFile(const std::string& full_filename) : impl_(std::make_unique<VideoFile::Impl>(full_filename)) { }
VideoFile::~VideoFile() = default;
bool VideoFile::is_open() const { return impl_->is_open(); }
bool VideoFile::is_video() const { return impl_->is_video(); }
const std::string& VideoFile::filename() const { return impl_->filename(); }
const std::string& VideoFile::file_format() const { return impl_->file_format(); }
CodecContext* VideoFile::audio_codec_context() const { return impl_->audio_codec_context(); }
CodecContext* VideoFile::video_codec_context() const { return impl_->video_codec_context(); }
bool VideoFile::has_audio_stream() const { return impl_->has_audio_stream(); }
bool VideoFile::has_video_stream() const { return impl_->has_video_stream(); }
void VideoFile::set_dump_first_frame(bool dump_frame) { impl_->set_dump_first_frame(dump_frame); }
std::unique_ptr<Frame> VideoFile::read_next_frame() { return impl_->read_next_frame(); }
clock::VideoDuration VideoFile::duration() const { return impl_->duration(); }
std::string VideoFile::format_duration() const { return impl_->format_duration(); }
bool VideoFile::seek_position(clock::PlaybackPosition position) { return impl_->seek_position(position); }

}  // namespace video_trimmer::video_file

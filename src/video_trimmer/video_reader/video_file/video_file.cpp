#include "video_file.hpp"

#include <filesystem>
#include <stdexcept>

extern "C" {
#include "libavcodec/packet.h"
#include "libavformat/avformat.h"
#include "libavutil/rational.h"
}

#include "auto_delete_resource.hpp"
#include "video_trimmer/error/error.hpp"

namespace video_trimmer::video_reader::video_file {

class VideoFile::Impl {
public:
    explicit Impl(const std::string& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] bool is_video() const;

    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }

    [[nodiscard]] codec_context::CodecContext* audio_codec_context() const { return audio_codec_context_.get(); }
    [[nodiscard]] codec_context::CodecContext* video_codec_context() const { return video_codec_context_.get(); }

    [[nodiscard]] bool has_audio_stream() const { return audio_codec_context_ != nullptr; };
    [[nodiscard]] bool has_video_stream() const { return video_codec_context_ != nullptr; };

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position);

    void set_dump_first_frame(bool dump_frame) { dump_first_frame_ = dump_frame; }

private:
    AutoDeleteResource<AVFormatContext> format_context_;
    AutoDeleteResource<AVPacket> packet_;

    std::unique_ptr<codec_context::CodecContext> audio_codec_context_;
    std::unique_ptr<codec_context::CodecContext> video_codec_context_;

    std::string filename_without_path_;
    std::string file_format_;

    bool is_open_ = false;

    [[nodiscard]] int open_file(const std::string& full_filename);

    [[nodiscard]] std::unique_ptr<codec_context::CodecContext> find_best_stream(codec_context::CodecContext::StreamType type);

    [[nodiscard]] std::unique_ptr<frame::Frame> decode_video_packet(AVPacket* packet);

    bool dump_first_frame_ = false;
};

VideoFile::Impl::Impl(const std::string& full_filename)
{
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
    audio_codec_context_ = find_best_stream(codec_context::CodecContext::StreamType::audio);
    video_codec_context_ = find_best_stream(codec_context::CodecContext::StreamType::video);

    // a missing audio stream is fine, but we are looking for at least a video stream
    if (!video_codec_context_)
        return -1;

    return 0;
}

bool VideoFile::Impl::is_video() const
{
    return is_open() && has_video_stream() && video_codec_context_->fps() > 0.0f;
}

std::unique_ptr<codec_context::CodecContext> VideoFile::Impl::find_best_stream(codec_context::CodecContext::StreamType type)
{
    // find decoder for stream and allocate codec context for decoder
    const AVMediaType media_type = type == codec_context::CodecContext::StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    AVStream* stream = format_context_->streams[stream_index];

    if (!stream)
        return nullptr;

    try {
        return std::make_unique<codec_context::CodecContext>(stream);
    } catch (const std::exception&) {
        return nullptr;
    }
}

std::unique_ptr<frame::Frame> VideoFile::Impl::read_next_frame(const double playback_position)
{
    // read until we get at least one video frame
    while (true) {
        if (av_read_frame(format_context_.get(), packet_.get()) < 0)
            break;

        // process only interesting packets, drop the rest
        if (packet_->stream_index == video_codec_context_->stream_index()) {
            std::unique_ptr<frame::Frame> frame = decode_video_packet(packet_.get());
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

std::unique_ptr<frame::Frame> VideoFile::Impl::decode_video_packet(AVPacket* packet)
{
    // send packet to the decoder
    if (video_codec_context_->send_packet_to_decoder(packet) < 0)
        return nullptr;

    // get available frame from the decoder
    std::unique_ptr<frame::Frame> frame = video_codec_context_->receive_frame_from_decoder(video_codec_context_->stream_time_base());

    if (dump_first_frame_ && frame) {
        frame->dump_to_file(filename_without_path_);
        dump_first_frame_ = false;
    }

    return frame;
}

VideoFile::VideoFile(const std::string& full_filename) : impl_(std::make_unique<VideoFile::Impl>(full_filename)) { }
VideoFile::~VideoFile() = default;
bool VideoFile::is_open() const { return impl_->is_open(); }
bool VideoFile::is_video() const { return impl_->is_video(); }
const std::string& VideoFile::filename() const { return impl_->filename(); }
const std::string& VideoFile::file_format() const { return impl_->file_format(); }
codec_context::CodecContext* VideoFile::audio_codec_context() const { return impl_->audio_codec_context(); }
codec_context::CodecContext* VideoFile::video_codec_context() const { return impl_->video_codec_context(); }
bool VideoFile::has_audio_stream() const { return impl_->has_audio_stream(); }
bool VideoFile::has_video_stream() const { return impl_->has_video_stream(); }
void VideoFile::set_dump_first_frame(bool dump_frame) { impl_->set_dump_first_frame(dump_frame); }
std::unique_ptr<frame::Frame> VideoFile::read_next_frame(double playback_position) { return impl_->read_next_frame(playback_position); }

}  // namespace video_trimmer::video_reader::video_file

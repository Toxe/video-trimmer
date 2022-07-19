#include "video_file.hpp"

#include <filesystem>
#include <stdexcept>

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/rational.h"
}

#include "auto_delete_resource.hpp"
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

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position, int scale_width, int scale_height);

    void set_dump_first_frame(bool dump_frame) { dump_first_frame_ = dump_frame; }

private:
    AutoDeleteResource<AVFormatContext> format_context_;

    std::unique_ptr<stream_info::StreamInfo> audio_stream_info_;
    std::unique_ptr<stream_info::StreamInfo> video_stream_info_;

    std::unique_ptr<packet::Packet> packet_;

    std::string filename_without_path_;
    std::string file_format_;

    bool is_open_ = false;

    [[nodiscard]] int open_file(const std::string_view& full_filename);

    [[nodiscard]] std::unique_ptr<stream_info::StreamInfo> find_best_stream(stream_info::StreamInfo::StreamType type);

    [[nodiscard]] std::unique_ptr<frame::Frame> decode_video_packet(packet::Packet* packet, int scale_width, int scale_height);

    [[nodiscard]] double stream_time_base(int stream_index) const;

    bool dump_first_frame_ = false;
};

VideoFile::Impl::Impl(const std::string_view& full_filename)
    : packet_(std::make_unique<packet::Packet>())

{
    is_open_ = open_file(full_filename) == 0;
}

int VideoFile::Impl::open_file(const std::string_view& full_filename)
{
    std::filesystem::path path{full_filename};

    if (!std::filesystem::exists(path))
        return error::show_error("file not found");

    filename_without_path_ = path.filename().string();

    // allocate format context
    format_context_ = AutoDeleteResource<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext* ctx) { avformat_close_input(&ctx); });

    if (!format_context_)
        return error::show_error("avformat_find_stream_info");

    // open input file
    int ret = avformat_open_input(format_context_.get_ptr(), full_filename.data(), nullptr, nullptr);

    if (ret < 0)
        return error::show_error("avformat_open_input", ret);

    // load stream info
    ret = avformat_find_stream_info(format_context_.get(), nullptr);

    if (ret < 0)
        return error::show_error("avformat_find_stream_info", ret);

    file_format_ = format_context_->iformat->long_name;

    // find best audio and video stream
    audio_stream_info_ = find_best_stream(stream_info::StreamInfo::StreamType::audio);
    video_stream_info_ = find_best_stream(stream_info::StreamInfo::StreamType::video);

    // a missing audio stream is fine, but we are looking for at least a video stream
    if (!video_stream_info_)
        return -1;

    return 0;
}

bool VideoFile::Impl::is_video() const
{
    return is_open() && has_video_stream() && video_stream_info_->codec_context()->fps() > 0.0f;
}

std::unique_ptr<stream_info::StreamInfo> VideoFile::Impl::find_best_stream(stream_info::StreamInfo::StreamType type)
{
    // find decoder for stream and allocate codec context for decoder
    const AVMediaType media_type = type == stream_info::StreamInfo::StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    AVStream* stream = format_context_->streams[stream_index];

    if (!stream)
        return nullptr;

    try {
        std::unique_ptr<codec_context::CodecContext> codec_context = std::make_unique<codec_context::CodecContext>(stream);

        if (!codec_context)
            return nullptr;

        return std::make_unique<stream_info::StreamInfo>(std::move(codec_context), stream->index);
    } catch (const std::exception&) {
        return nullptr;
    }
}

std::unique_ptr<frame::Frame> VideoFile::Impl::read_next_frame(const double playback_position, const int scale_width, const int scale_height)
{
    // read until we get at least one video frame
    while (true) {
        if (av_read_frame(format_context_.get(), packet_->packet()) < 0)
            break;

        // process only interesting packets, drop the rest
        if (packet_->stream_index() == video_stream_info()->stream_index()) {
            std::unique_ptr<frame::Frame> frame = decode_video_packet(packet_.get(), scale_width, scale_height);
            packet_->unref();
            return frame;
        } else if (packet_->stream_index() == audio_stream_info()->stream_index()) {
            // TODO: decode audio packet
            packet_->unref();
        } else {
            packet_->unref();
        }
    }

    return nullptr;
}

std::unique_ptr<frame::Frame> VideoFile::Impl::decode_video_packet(packet::Packet* packet, const int scale_width, const int scale_height)
{
    // send packet to the decoder
    if (video_stream_info_->codec_context()->send_packet_to_decoder(packet) < 0)
        return nullptr;

    // get available frame from the decoder
    std::unique_ptr<frame::Frame> frame = video_stream_info_->codec_context()->receive_frame_from_decoder(stream_time_base(video_stream_info_->stream_index()), scale_width, scale_height);

    if (!frame)
        return nullptr;

    if (dump_first_frame_) {
        frame->dump_to_file(filename_without_path_);
        dump_first_frame_ = false;
    }

    // copy decoded frame to image buffer
    frame->image_copy();

    return frame;
}

double VideoFile::Impl::stream_time_base(const int stream_index) const
{
    const AVStream* stream = format_context_->streams[stream_index];
    return av_q2d(stream->time_base);
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
void VideoFile::set_dump_first_frame(bool dump_frame) { impl_->set_dump_first_frame(dump_frame); }
std::unique_ptr<frame::Frame> VideoFile::read_next_frame(double playback_position, const int scale_width, const int scale_height) { return impl_->read_next_frame(playback_position, scale_width, scale_height); }

}  // namespace video_trimmer::video_reader::video_file

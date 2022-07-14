#include "format_context.hpp"

#include <stdexcept>

extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libavutil/rational.h"
}

#include "auto_delete_resource.hpp"

namespace video_trimmer::video_reader::format_context {

class FormatContext::Impl {
public:
    explicit Impl(const std::string_view& filename);

    [[nodiscard]] double stream_time_base(int stream_index) const;

    [[nodiscard]] std::string format() const;

    [[nodiscard]] AVStream* find_best_stream(StreamType type);
    [[nodiscard]] int read_frame(packet::Packet* packet);

private:
    AutoDeleteResource<AVFormatContext> format_context_;
};

FormatContext::Impl::Impl(const std::string_view& filename)
{
    // allocate format context
    format_context_ = AutoDeleteResource<AVFormatContext>(avformat_alloc_context(), [](AVFormatContext* ctx) { avformat_close_input(&ctx); });

    if (!format_context_)
        throw std::runtime_error("avformat_alloc_context");

    // open input file
    int ret = avformat_open_input(format_context_.get_ptr(), filename.data(), nullptr, nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_open_input");

    // load stream info
    ret = avformat_find_stream_info(format_context_.get(), nullptr);

    if (ret < 0)
        throw std::runtime_error("avformat_find_stream_info");
}

double FormatContext::Impl::stream_time_base(const int stream_index) const
{
    const AVStream* stream = format_context_->streams[stream_index];
    return av_q2d(stream->time_base);
}

std::string FormatContext::Impl::format() const
{
    return format_context_->iformat->long_name;
}

AVStream* FormatContext::Impl::find_best_stream(FormatContext::StreamType type)
{
    const AVMediaType media_type = type == FormatContext::StreamType::audio ? AVMEDIA_TYPE_AUDIO : AVMEDIA_TYPE_VIDEO;
    const int stream_index = av_find_best_stream(format_context_.get(), media_type, -1, -1, nullptr, 0);

    if (stream_index < 0)
        return nullptr;

    // find decoder for stream
    return format_context_->streams[stream_index];
}

int FormatContext::Impl::read_frame(packet::Packet* packet)
{
    return av_read_frame(format_context_.get(), packet->packet());
}

FormatContext::FormatContext(const std::string_view& filename) : impl_(std::make_unique<FormatContext::Impl>(filename)) { }
FormatContext::~FormatContext() = default;
double FormatContext::stream_time_base(int stream_index) const { return impl_->stream_time_base(stream_index); }
std::string FormatContext::format() const { return impl_->format(); }
AVStream* FormatContext::find_best_stream(FormatContext::StreamType type) { return impl_->find_best_stream(type); }
int FormatContext::read_frame(packet::Packet* packet) { return impl_->read_frame(packet); }

}  // namespace video_trimmer::video_reader::format_context

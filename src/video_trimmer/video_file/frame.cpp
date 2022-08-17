#include "frame.hpp"

#include <array>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include "fmt/core.h"

extern "C" {
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
}

#include "../logger/logger.hpp"
#include "auto_delete_resource.hpp"

namespace video_trimmer::video_file {

class Frame::Impl {
public:
    enum class FrameType {
        audio,
        video
    };

    Impl(FrameType frame_type, double timestamp, double duration, Size size, PixelFormat pixel_format);

    [[nodiscard]] static std::unique_ptr<Frame::Impl> create_audio_frame();
    [[nodiscard]] static std::unique_ptr<Frame::Impl> create_video_frame(Size size, PixelFormat pixel_format, double timestamp, double duration, char picture_type);

    void update_from_frame(double stream_time_base);

    [[nodiscard]] bool is_audio_frame() const { return frame_type_ == FrameType::audio; }
    [[nodiscard]] bool is_video_frame() const { return frame_type_ == FrameType::video; }

    [[nodiscard]] Size size() const { return size_; }

    [[nodiscard]] double timestamp() const { return timestamp_; }
    [[nodiscard]] double duration() const { return duration_; }

    [[nodiscard]] uint8_t** data() { return frame_->data; }
    [[nodiscard]] int* linesizes() { return frame_->linesize; }

    [[nodiscard]] AVFrame* frame() { return frame_.get(); }
    [[nodiscard]] const PixelFormat& pixel_format() const { return pixel_format_; };

    [[nodiscard]] std::string description() const;

    void dump_to_file(const std::string& filename);

private:
    AutoDeleteResource<AVFrame> frame_;

    FrameType frame_type_;

    double timestamp_;
    double duration_;
    int64_t pts_ = 0;
    int64_t pkt_duration_ = 0;

    Size size_;
    PixelFormat pixel_format_;
    char picture_type_ = '?';
};

Frame::Impl::Impl(FrameType frame_type, double timestamp, double duration, Size size, PixelFormat pixel_format)
    : frame_type_(frame_type), timestamp_(timestamp), duration_(duration), size_(size), pixel_format_(pixel_format)
{
    frame_ = AutoDeleteResource<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });

    if (!frame_)
        throw std::runtime_error("av_frame_alloc");
}

std::unique_ptr<Frame::Impl> Frame::Impl::create_audio_frame()
{
    return std::make_unique<Frame::Impl>(FrameType::audio, 0.0, 0.0, Size{0, 0}, PixelFormat{});
}

std::unique_ptr<Frame::Impl> Frame::Impl::create_video_frame(Size size, PixelFormat pixel_format, double timestamp, double duration, char picture_type)
{
    auto frame = std::make_unique<Frame::Impl>(FrameType::video, timestamp, duration, size, pixel_format);
    frame->picture_type_ = picture_type;

    return frame;
}

void Frame::Impl::update_from_frame(double stream_time_base)
{
    timestamp_ = static_cast<double>(frame_->best_effort_timestamp) * stream_time_base;
    duration_ = static_cast<double>(frame_->pkt_duration) * stream_time_base;
    pts_ = frame_->pts;
    pkt_duration_ = frame_->pkt_duration;
    picture_type_ = av_get_picture_type_char(frame_->pict_type);
}

void Frame::Impl::dump_to_file(const std::string& filename)
{
    std::filesystem::path out_filename{filename};
    out_filename.replace_filename(fmt::format("{}_{}x{}_{}.raw", out_filename.stem().string(), frame_->width, frame_->height, pixel_format_.name()));

    video_trimmer::logger::log_info(fmt::format("dump first video frame to file: {}", out_filename.string()));

    const int buffer_size = av_image_get_buffer_size(static_cast<AVPixelFormat>(pixel_format_.av_pixel_format()), frame_->width, frame_->height, 1);
    std::unique_ptr<uint8_t[]> buffer = std::make_unique<uint8_t[]>(static_cast<size_t>(buffer_size));
    const int bytes_copied = av_image_copy_to_buffer(buffer.get(), buffer_size, frame_->data, frame_->linesize, static_cast<AVPixelFormat>(pixel_format_.av_pixel_format()), frame_->width, frame_->height, 1);

    if (bytes_copied != buffer_size)
        throw std::runtime_error("av_image_copy_to_buffer error");

    std::ofstream out{out_filename, std::ofstream::binary};

    if (!out.is_open())
        throw std::runtime_error(fmt::format("unable to open output file: {}", out_filename.string()));

    out.write(reinterpret_cast<const char*>(buffer.get()), buffer_size);
}

std::string Frame::Impl::description() const
{
    if (is_video_frame())
        return fmt::format("[Frame V{} @{:.3f} {:.4f}s, {}x{}, pts={}, pkt_duration={}]", picture_type_, timestamp_, duration_, size_.width, size_.height, pts_, pkt_duration_);
    else
        return fmt::format("[Frame A @{:.3f} {:.4f}s]", timestamp_, duration_);
}

Frame::Frame(std::unique_ptr<Impl> impl) : impl_(std::move(impl)) { }
Frame::~Frame() = default;
std::unique_ptr<Frame> Frame::create_audio_frame() { return std::make_unique<Frame>(Frame::Impl::create_audio_frame()); }
std::unique_ptr<Frame> Frame::create_video_frame() { return create_video_frame(Size{}, PixelFormat{}); }
std::unique_ptr<Frame> Frame::create_video_frame(Size size, PixelFormat pixel_format, double timestamp, double duration, char picture_type) { return std::make_unique<Frame>(Frame::Impl::create_video_frame(size, pixel_format, timestamp, duration, picture_type)); }
void Frame::update_from_frame(double stream_time_base) { impl_->update_from_frame(stream_time_base); }
bool Frame::is_audio_frame() const { return impl_->is_audio_frame(); }
bool Frame::is_video_frame() const { return impl_->is_video_frame(); }
Size Frame::size() const { return impl_->size(); }
double Frame::timestamp() const { return impl_->timestamp(); }
double Frame::duration() const { return impl_->duration(); }
uint8_t** Frame::data() { return impl_->data(); }
int* Frame::linesizes() { return impl_->linesizes(); }
AVFrame* Frame::frame() { return impl_->frame(); }
PixelFormat Frame::pixel_format() const { return impl_->pixel_format(); }
std::string Frame::description() const { return impl_->description(); }
void Frame::dump_to_file(const std::string& filename) { impl_->dump_to_file(filename); }

}  // namespace video_trimmer::video_file

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

#include "auto_delete_resource.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_file {

class Frame::Impl {
public:
    Impl(Size size, PixelFormat pixel_format);

    [[nodiscard]] bool is_audio_frame() const { return frame_type_ == FrameType::audio; }
    [[nodiscard]] bool is_video_frame() const { return frame_type_ == FrameType::video; }

    [[nodiscard]] Size size() const { return size_; }

    [[nodiscard]] double timestamp() const { return timestamp_; }
    void set_timestamp(double timestamp) { timestamp_ = timestamp; }

    [[nodiscard]] uint8_t** data() { return frame_->data; }
    [[nodiscard]] int* linesizes() { return frame_->linesize; }

    [[nodiscard]] AVFrame* frame() { return frame_.get(); }
    [[nodiscard]] const PixelFormat& pixel_format() const { return pixel_format_; };

    void dump_to_file(const std::string& filename);

private:
    enum class FrameType {
        audio,
        video
    };

    FrameType frame_type_;
    PixelFormat pixel_format_;
    Size size_;

    double timestamp_ = 0.0;

    AutoDeleteResource<AVFrame> frame_;
};

Frame::Impl::Impl(Size size, PixelFormat pixel_format)
    : frame_type_{FrameType::video}, pixel_format_{pixel_format}, size_{size}
{
    frame_ = AutoDeleteResource<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });

    if (!frame_)
        throw std::runtime_error("av_frame_alloc");
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

Frame::Frame(Size size, PixelFormat pixel_format) : impl_(std::make_unique<Frame::Impl>(size, std::move(pixel_format))) { }
Frame::~Frame() = default;
bool Frame::is_audio_frame() const { return impl_->is_audio_frame(); }
bool Frame::is_video_frame() const { return impl_->is_video_frame(); }
Size Frame::size() const { return impl_->size(); }
double Frame::timestamp() const { return impl_->timestamp(); }
void Frame::set_timestamp(double timestamp) { impl_->set_timestamp(timestamp); }
uint8_t** Frame::data() { return impl_->data(); }
int* Frame::linesizes() { return impl_->linesizes(); }
AVFrame* Frame::frame() { return impl_->frame(); }
PixelFormat Frame::pixel_format() const { return impl_->pixel_format(); }
void Frame::dump_to_file(const std::string& filename) { impl_->dump_to_file(filename); }

}  // namespace video_trimmer::video_file

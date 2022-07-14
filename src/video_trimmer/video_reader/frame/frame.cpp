#include "frame.hpp"

#include <array>
#include <cstdint>
#include <span>
#include <stdexcept>

extern "C" {
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
#include "libavutil/mem.h"
}

#include "auto_delete_resource.hpp"

namespace video_trimmer::video_reader::frame {

class Frame::Impl {
public:
    Impl(int width, int height, int scaled_width, int scaled_height, AVPixelFormat pixel_format);
    ~Impl();

    [[nodiscard]] double timestamp() const { return timestamp_; };
    void set_timestamp(double timestamp) { timestamp_ = timestamp; };

    [[nodiscard]] int src_width() const { return src_width_; }
    [[nodiscard]] int src_height() const { return src_height_; }
    [[nodiscard]] int dst_width() const { return dst_width_; }
    [[nodiscard]] int dst_height() const { return dst_height_; }

    [[nodiscard]] AVFrame* frame() { return frame_.get(); }

    [[nodiscard]] uint8_t** src_data() { return img_buf_data_.data(); }
    [[nodiscard]] uint8_t** dst_data() { return dst_buf_data_.data(); }
    [[nodiscard]] int* src_linesizes() { return img_buf_linesize_.data(); }
    [[nodiscard]] int* dst_linesizes() { return dst_buf_linesize_.data(); }

    [[nodiscard]] std::span<const uint8_t> pixels();

    void image_copy();

private:
    int src_width_;
    int src_height_;
    int dst_width_;
    int dst_height_;

    double timestamp_ = 0.0;

    AutoDeleteResource<AVFrame> frame_;

    std::array<uint8_t*, 4> img_buf_data_ = {nullptr};
    std::array<uint8_t*, 4> dst_buf_data_ = {nullptr};
    std::array<int, 4> img_buf_linesize_ = {0};
    std::array<int, 4> dst_buf_linesize_ = {0};

    AVPixelFormat src_pixel_format_;
};

Frame::Impl::Impl(const int width, const int height, const int scaled_width, const int scaled_height, AVPixelFormat pixel_format)
    : src_width_(width), src_height_(height), src_pixel_format_(pixel_format)
{
    // only scale down, never scale frames up to be bigger than the source frame
    if (scaled_width <= src_width_ && scaled_height <= src_height_) {
        dst_width_ = scaled_width;
        dst_height_ = scaled_height;
    } else {
        dst_width_ = src_width_;
        dst_height_ = src_height_;
    }

    frame_ = AutoDeleteResource<AVFrame>(av_frame_alloc(), [](AVFrame* p) { av_frame_free(&p); });

    if (!frame_)
        throw std::runtime_error("av_frame_alloc");

    // allocate buffer for decoded source images
    int buf_size = av_image_alloc(img_buf_data_.data(), img_buf_linesize_.data(), src_width(), src_height(), src_pixel_format_, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");

    // allocate buffer for scaled output images
    buf_size = av_image_alloc(dst_buf_data_.data(), dst_buf_linesize_.data(), dst_width(), dst_height(), AV_PIX_FMT_RGBA, 1);

    if (buf_size < 0)
        throw std::runtime_error("av_image_alloc");
}

Frame::Impl::~Impl()
{
    av_freep(dst_buf_data_.data());
    av_freep(img_buf_data_.data());
}

std::span<const uint8_t> Frame::Impl::pixels()
{
    return std::span<const uint8_t>{dst_buf_data_[0], static_cast<std::size_t>(4 * dst_width() * dst_height())};
}

void Frame::Impl::image_copy()
{
    av_image_copy(src_data(), src_linesizes(), const_cast<const uint8_t**>(frame_->data), &frame_->linesize[0], src_pixel_format_, src_width(), src_height());
    av_frame_unref(frame_.get());
}

Frame::Frame(int width, int height, int scaled_width, int scaled_height, AVPixelFormat pixel_format) : impl_(std::make_unique<Frame::Impl>(width, height, scaled_width, scaled_height, pixel_format)) { }
Frame::~Frame() = default;
double Frame::timestamp() const { return impl_->timestamp(); }
void Frame::set_timestamp(double timestamp) { impl_->set_timestamp(timestamp); }
int Frame::src_width() const { return impl_->src_width(); }
int Frame::src_height() const { return impl_->src_height(); }
int Frame::dst_width() const { return impl_->dst_width(); }
int Frame::dst_height() const { return impl_->dst_height(); }
AVFrame* Frame::frame() { return impl_->frame(); }
uint8_t** Frame::src_data() { return impl_->src_data(); }
uint8_t** Frame::dst_data() { return impl_->dst_data(); }
int* Frame::src_linesizes() { return impl_->src_linesizes(); }
int* Frame::dst_linesizes() { return impl_->dst_linesizes(); }
std::span<const uint8_t> Frame::pixels() { return impl_->pixels(); }
void Frame::image_copy() { impl_->image_copy(); }

}  // namespace video_trimmer::video_reader::frame

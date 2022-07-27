#include "texture.hpp"

#include <cassert>
#include <stdexcept>

#include "SDL2/SDL_render.h"
#include "fmt/core.h"

extern "C" {
#include "libavutil/pixfmt.h"
}

#include "auto_delete_resource.hpp"
#include "graphics.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::graphics {

class Texture::Impl {
public:
    Impl(Graphics& graphics, Size size, AVPixelFormat pixel_format);

    [[nodiscard]] Size size() const { return size_; }

    [[nodiscard]] bool is_compatible(Size size, AVPixelFormat pixel_format) const;

    void update(video_file::Frame* video_frame);
    void draw(Graphics& graphics, Position dst_position, Size dst_size);

private:
    AutoDeleteResource<SDL_Texture> sdl_texture_;

    Size size_{};
    float aspect_ratio_;

    SDL_PixelFormatEnum sdl_pixel_format_ = SDL_PIXELFORMAT_UNKNOWN;
    AVPixelFormat av_pixel_format_ = AV_PIX_FMT_NONE;

    [[nodiscard]] static SDL_PixelFormatEnum get_sdl_pixel_format(AVPixelFormat av_pixel_format);
};

Texture::Impl::Impl(Graphics& graphics, Size size, AVPixelFormat pixel_format)
{
    size_ = size;
    aspect_ratio_ = static_cast<float>(size_.width) / static_cast<float>(size_.height);

    av_pixel_format_ = pixel_format;
    sdl_pixel_format_ = Texture::Impl::get_sdl_pixel_format(av_pixel_format_);

    if (sdl_pixel_format_ == SDL_PIXELFORMAT_UNKNOWN)
        throw std::runtime_error(fmt::format("pixel format {} is not supported", video_file::Frame::pixel_format_name(pixel_format)));

    sdl_texture_ = AutoDeleteResource<SDL_Texture>(graphics.create_texture(sdl_pixel_format_, size_), [](SDL_Texture* tex) { SDL_DestroyTexture(tex); });

    if (sdl_texture_.get())
        logger::log_debug(fmt::format("created new {}x{} {} render texture", size_.width, size_.height, video_file::Frame::pixel_format_name(pixel_format)));
}

bool Texture::Impl::is_compatible(Size size, AVPixelFormat pixel_format) const
{
    if (!sdl_texture_)
        return false;

    if (size_ != size)
        return false;

    return av_pixel_format_ == pixel_format;
}

void Texture::Impl::update(video_file::Frame* video_frame)
{
    assert(sdl_texture_);
    assert(video_frame);

    uint8_t** data = video_frame->data();
    int* linesizes = video_frame->linesizes();

    if (SDL_UpdateYUVTexture(sdl_texture_.get(), nullptr, data[0], linesizes[0], data[1], linesizes[1], data[2], linesizes[2]) < 0)
        logger::log_error(fmt::format("unable to update texture: {}", SDL_GetError()));
}

void Texture::Impl::draw(Graphics& graphics, Position dst_position, Size dst_size)
{
    assert(sdl_texture_);

    // scale texture to fit into the destination rect (keeping its aspect ratio) and center it
    const float dst_aspect_ratio = static_cast<float>(dst_size.width) / static_cast<float>(dst_size.height);

    if (dst_aspect_ratio < aspect_ratio_) {
        const int old_height = dst_size.height;
        dst_size.height = static_cast<int>(static_cast<float>(dst_size.width) / aspect_ratio_);
        dst_position.y += (old_height - dst_size.height) / 2;
    } else {
        const int old_width = dst_size.width;
        dst_size.width = static_cast<int>(static_cast<float>(dst_size.height) * aspect_ratio_);
        dst_position.x += (old_width - dst_size.width) / 2;
    }

    graphics.render_texture(sdl_texture_.get(), dst_position, dst_size);
}

SDL_PixelFormatEnum Texture::Impl::get_sdl_pixel_format(AVPixelFormat av_pixel_format)
{
    switch (av_pixel_format) {
    case AV_PIX_FMT_YUV420P:
        return SDL_PIXELFORMAT_IYUV;
    case AV_PIX_FMT_YUYV422:
        return SDL_PIXELFORMAT_YUY2;
    case AV_PIX_FMT_UYVY422:
        return SDL_PIXELFORMAT_UYVY;
    default:
        return SDL_PIXELFORMAT_UNKNOWN;
    }
}

Texture::Texture(Graphics& graphics, Size size, AVPixelFormat pixel_format) : impl_(std::make_unique<Texture::Impl>(graphics, size, pixel_format)) { }
Texture::~Texture() = default;
Size Texture::size() const { return impl_->size(); }
bool Texture::is_compatible(Size size, AVPixelFormat pixel_format) const { return impl_->is_compatible(size, pixel_format); }
void Texture::update(video_file::Frame* video_frame) { impl_->update(video_frame); }
void Texture::draw(Graphics& graphics, Position dst_position, Size dst_size) { impl_->draw(graphics, dst_position, dst_size); }

}  // namespace video_trimmer::graphics

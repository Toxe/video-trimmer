#include "texture.hpp"

#include <cassert>
#include <stdexcept>

#include "SDL2/SDL_render.h"
#include "fmt/core.h"

#include "auto_delete_resource.hpp"
#include "graphics.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::graphics {

class Texture::Impl {
public:
    Impl(Graphics& graphics, Size size, video_file::PixelFormat pixel_format);

    [[nodiscard]] Size size() const { return size_; }

    [[nodiscard]] bool is_compatible(Size size, video_file::PixelFormat pixel_format) const;

    void update(video_file::Frame* video_frame);
    void draw(Graphics& graphics, Position dst_position, Size dst_size);

private:
    AutoDeleteResource<SDL_Texture> sdl_texture_;

    Size size_{};
    float aspect_ratio_;

    video_file::PixelFormat pixel_format_;
    SDL_PixelFormatEnum sdl_pixel_format_ = SDL_PIXELFORMAT_UNKNOWN;

    bool is_empty_ = true;
};

Texture::Impl::Impl(Graphics& graphics, Size size, video_file::PixelFormat pixel_format)
    : size_(size),
      aspect_ratio_(static_cast<float>(size_.width) / static_cast<float>(size_.height)),
      pixel_format_(pixel_format),
      sdl_pixel_format_(static_cast<SDL_PixelFormatEnum>(pixel_format_.get_compatible_sdl_pixel_format()))
{
    if (sdl_pixel_format_ == SDL_PIXELFORMAT_UNKNOWN)
        throw std::runtime_error(fmt::format("pixel format {} is not supported", pixel_format.name()));

    sdl_texture_ = AutoDeleteResource<SDL_Texture>(graphics.create_texture(sdl_pixel_format_, size_), [](SDL_Texture* tex) { SDL_DestroyTexture(tex); });

    if (sdl_texture_.get())
        logger::log_debug(fmt::format("created new {}x{} {} render texture", size_.width, size_.height, pixel_format.name()));
}

bool Texture::Impl::is_compatible(Size size, video_file::PixelFormat pixel_format) const
{
    if (!sdl_texture_)
        return false;

    if (size_ != size)
        return false;

    return pixel_format_ == pixel_format;
}

void Texture::Impl::update(video_file::Frame* video_frame)
{
    assert(sdl_texture_.get() != nullptr);
    assert(video_frame);

    uint8_t** data = video_frame->data();
    int* linesizes = video_frame->linesizes();

    if (SDL_UpdateYUVTexture(sdl_texture_.get(), nullptr, data[0], linesizes[0], data[1], linesizes[1], data[2], linesizes[2]) < 0)
        logger::log_error(fmt::format("unable to update texture: {}", SDL_GetError()));
    else
        is_empty_ = false;
}

void Texture::Impl::draw(Graphics& graphics, Position dst_position, Size dst_size)
{
    assert(sdl_texture_.get() != nullptr);

    // only draw the texture if it has image data
    if (is_empty_)
        return;

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

Texture::Texture(Graphics& graphics, Size size, video_file::PixelFormat pixel_format) : impl_(std::make_unique<Texture::Impl>(graphics, size, pixel_format)) { }
Texture::~Texture() = default;
Size Texture::size() const { return impl_->size(); }
bool Texture::is_compatible(Size size, video_file::PixelFormat pixel_format) const { return impl_->is_compatible(size, pixel_format); }
void Texture::update(video_file::Frame* video_frame) { impl_->update(video_frame); }
void Texture::draw(Graphics& graphics, Position dst_position, Size dst_size) { impl_->draw(graphics, dst_position, dst_size); }

}  // namespace video_trimmer::graphics

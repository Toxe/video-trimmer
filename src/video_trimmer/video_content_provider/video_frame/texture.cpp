#include "texture.hpp"

namespace video_trimmer::video_content_provider::video_frame {

Texture::Texture(frame::Frame* frame)
{
    const auto rgba = frame->pixels();

    if (!rgba.empty()) {
        texture_ = std::make_unique<sf::Texture>();
        texture_->create(frame->dst_width(), frame->dst_height());
        texture_->update(rgba.data());
    }
}

std::unique_ptr<sf::Texture> Texture::release_texture()
{
    return std::move(texture_);
}

}  // namespace video_trimmer::video_content_provider::video_frame

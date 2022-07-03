#include "texture.hpp"

Texture::Texture(Frame* frame)
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

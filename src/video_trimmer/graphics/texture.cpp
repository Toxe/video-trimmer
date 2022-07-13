#include "texture.hpp"

#include "graphics.hpp"

namespace video_trimmer::graphics {

Texture::Texture(SDL_Texture* texture, ImageSize size)
    : size_(size), sdl_texture_(texture)
{
}

Texture::~Texture()
{
    if (sdl_texture_)
        Graphics::destroy_texture(this);
}

}  // namespace video_trimmer::graphics

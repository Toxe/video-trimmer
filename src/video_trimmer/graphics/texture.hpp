#pragma once

#include "types.h"

struct SDL_Texture;

namespace video_trimmer::graphics {

class Texture {
public:
    Texture(SDL_Texture* texture, ImageSize size);
    ~Texture();

    [[nodiscard]] ImageSize size() const { return size_; }
    [[nodiscard]] SDL_Texture* sdl_texture() const { return sdl_texture_; }

private:
    ImageSize size_;
    SDL_Texture* sdl_texture_;
};

}  // namespace video_trimmer::graphics

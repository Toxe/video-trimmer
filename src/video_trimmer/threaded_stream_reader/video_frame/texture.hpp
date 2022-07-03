#pragma once

#include <memory>

#include "SFML/Graphics/Texture.hpp"

#include "../frame/frame.hpp"

class Texture {
    std::unique_ptr<sf::Texture> texture_;

public:
    explicit Texture(Frame* frame);

    [[nodiscard]] std::unique_ptr<sf::Texture> release_texture();
};

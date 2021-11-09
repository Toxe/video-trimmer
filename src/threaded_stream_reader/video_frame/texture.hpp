#pragma once

#include <memory>

#include <SFML/Graphics/Texture.hpp>

class Frame;

class Texture {
    std::unique_ptr<sf::Texture> texture_;

public:
    Texture(Frame* frame);

    [[nodiscard]] std::unique_ptr<sf::Texture> release_texture();
};

#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "types.h"

class VideoFrame;

class VideoView {
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

public:
    VideoView();

    void render(sf::RenderWindow& window, ImagePosition video_view_position, ImageSize video_view_size, VideoFrame* video_frame);
};

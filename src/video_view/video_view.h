#pragma once

#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "types.h"

class VideoContentProvider;

class VideoView {
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

    bool needs_to_resize_texture(const ImageSize video_view_size);

public:
    VideoView();

    void render(sf::RenderWindow& window, const ImagePosition video_view_position, const ImageSize video_view_size, VideoContentProvider& video_content_provider);
};

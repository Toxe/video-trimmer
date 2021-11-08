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

    bool is_same_size(const sf::Vector2u size1, const ImageSize size2) const;

    void resize_texture(const ImageSize video_view_size);
    void render_video_frame(VideoFrame* video_frame);

public:
    VideoView();

    void render(sf::RenderWindow& window, const ImagePosition video_view_position, const ImageSize video_view_size, VideoFrame* video_frame);
};

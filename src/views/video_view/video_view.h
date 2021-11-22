#pragma once

#include <memory>
#include <tuple>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "types.h"

class VideoFrame;

class VideoView {
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

    std::tuple<ImagePosition, ImageSize> render_ui(const VideoFrame* video_frame);
    void render_content(sf::RenderWindow& window, const ImagePosition& video_view_position, const ImageSize& video_view_size, VideoFrame* video_frame);

public:
    VideoView();

    void render(sf::RenderWindow& window, VideoFrame* video_frame);
};

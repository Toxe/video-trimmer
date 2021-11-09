#include "video_view.h"

#include <fmt/core.h>

#include "logger/logger.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"

VideoView::VideoView()
{
    // create render texture and sprite that shows video frames
    sf::Image image;
    image.create(4, 4, sf::Color::Blue);

    texture_ = std::make_unique<sf::Texture>();
    texture_->loadFromImage(image);

    sprite_ = std::make_unique<sf::Sprite>();
    sprite_->setTexture(*texture_, true);
}

void VideoView::render(sf::RenderWindow& window, const ImagePosition video_view_position, const ImageSize video_view_size, VideoFrame* video_frame)
{
    if (video_frame) {
        // receive a new sprite texture
        texture_ = std::move(video_frame->texture()->release_texture());
        sprite_->setTexture(*texture_, true);
    }

    sprite_->setPosition(static_cast<float>(video_view_position.x), static_cast<float>(video_view_position.y));
    sprite_->setScale(static_cast<float>(video_view_size.width) / static_cast<float>(texture_->getSize().x), static_cast<float>(video_view_size.height) / static_cast<float>(texture_->getSize().y));

    window.draw(*sprite_);
}

#include "video_view.h"

#include "video/video_stream.hpp"

VideoView::VideoView()
{
    // create render texture and sprite that shows video frames
    sf::Image image;
    image.create(10, 10, sf::Color::Blue);

    texture_ = std::make_unique<sf::Texture>();
    texture_->loadFromImage(image);

    sprite_ = std::make_unique<sf::Sprite>();
    sprite_->setTexture(*texture_);
}

void VideoView::render(sf::RenderWindow& window, ImagePosition video_view_position, ImageSize video_view_size, VideoStream& video_stream)
{
    const auto texture_size = texture_->getSize();

    if (static_cast<int>(texture_size.x) != video_view_size.width || static_cast<int>(texture_size.y) != video_view_size.height) {
        sf::Image image;
        image.create(video_view_size.width, video_view_size.height, sf::Color::Blue);

        texture_->loadFromImage(image);
        sprite_->setTextureRect(sf::IntRect{0, 0, video_view_size.width, video_view_size.height});
    }

    video_stream.next_frame(*texture_);

    sprite_->setPosition(static_cast<float>(video_view_position.x), static_cast<float>(video_view_position.y));
    window.draw(*sprite_);
}

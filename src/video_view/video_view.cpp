#include "video_view.h"

#include "video/video_frame/video_frame.hpp"

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

void VideoView::render(sf::RenderWindow& window, const ImagePosition video_view_position, const ImageSize video_view_size, VideoFrame* video_frame)
{
    if (needs_to_resize_texture(video_view_size)) {
        sf::Image image;
        image.create(video_view_size.width, video_view_size.height, sf::Color::Blue);

        texture_->loadFromImage(image);
        sprite_->setTextureRect(sf::IntRect{0, 0, video_view_size.width, video_view_size.height});
    } else {
        if (video_frame) {
            auto pixels = video_frame->frame()->pixels();

            if (!pixels.empty())
                texture_->update(pixels.data());
        }
    }

    sprite_->setPosition(static_cast<float>(video_view_position.x), static_cast<float>(video_view_position.y));
    window.draw(*sprite_);
}

bool VideoView::needs_to_resize_texture(const ImageSize video_view_size)
{
    const auto& texture_size = texture_->getSize();

    return static_cast<int>(texture_size.x) != video_view_size.width || static_cast<int>(texture_size.y) != video_view_size.height;
}

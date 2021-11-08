#include "video_view.h"

#include <fmt/core.h>

#include "logger/logger.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"

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
    if (!is_same_size(texture_->getSize(), video_view_size))
        resize_texture(video_view_size);

    if (video_frame)
        render_video_frame(video_frame);

    sprite_->setPosition(static_cast<float>(video_view_position.x), static_cast<float>(video_view_position.y));
    window.draw(*sprite_);
}

bool VideoView::is_same_size(const sf::Vector2u size1, const ImageSize size2) const
{
    return static_cast<int>(size1.x) == size2.width && static_cast<int>(size1.y) == size2.height;
}

void VideoView::resize_texture(const ImageSize video_view_size)
{
    sf::Image image;
    image.create(video_view_size.width, video_view_size.height, sf::Color::Blue);

    texture_->loadFromImage(image);
    sprite_->setTextureRect(sf::IntRect{0, 0, video_view_size.width, video_view_size.height});
}

void VideoView::render_video_frame(VideoFrame* video_frame)
{
    const auto pixels = video_frame->frame()->pixels();

    if (pixels.empty())
        return;

    if (is_same_size(texture_->getSize(), {video_frame->width(), video_frame->height()})) {
        log_debug(fmt::format("update texture for {}", video_frame->print()));
        texture_->update(pixels.data());
    } else {
        log_debug(fmt::format("unable to show {}, texture size is {}x{}", video_frame->print(), texture_->getSize().x, texture_->getSize().y));
    }
}

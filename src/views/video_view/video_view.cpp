#include "video_view.h"

#include <fmt/core.h>
#include <imgui.h>

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
    render_ui();
    render_content(window, video_view_position, video_view_size, video_frame);
}

void VideoView::render_content(sf::RenderWindow& window, const ImagePosition& video_view_position, const ImageSize& video_view_size, VideoFrame* video_frame)
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

void VideoView::render_ui()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("video pane");

    ImGui::Text("Video");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

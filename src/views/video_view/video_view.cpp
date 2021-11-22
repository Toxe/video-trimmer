#include "video_view.h"

#include <fmt/core.h>
#include <imgui.h>

#include "logger/logger.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"
#include "ui/colors.h"
#include "ui/imgui_helpers.hpp"

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

void VideoView::render(sf::RenderWindow& window, VideoFrame* video_frame)
{
    const auto [view_position, view_size] = render_ui(video_frame);
    render_content(window, view_position, view_size, video_frame);
}

void VideoView::render_content(sf::RenderWindow& window, const ImagePosition& video_view_position, const ImageSize& video_view_size, VideoFrame* video_frame)
{
    if (video_frame) {
        // receive a new sprite texture
        texture_ = video_frame->texture()->release_texture();
        sprite_->setTexture(*texture_, true);
    }

    sprite_->setPosition(static_cast<float>(video_view_position.x), static_cast<float>(video_view_position.y));
    sprite_->setScale(static_cast<float>(video_view_size.width) / static_cast<float>(texture_->getSize().x), static_cast<float>(video_view_size.height) / static_cast<float>(texture_->getSize().y));

    window.draw(*sprite_);
}

std::tuple<ImagePosition, ImageSize> VideoView::render_ui(const VideoFrame* video_frame)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("video");

    // current view position and size
    const ImVec2 imgui_cursor_screen_pos = ImGui::GetCursorScreenPos();
    const ImVec2 imgui_window_size = ImGui::GetWindowSize();

    const ImagePosition view_position{static_cast<int>(imgui_cursor_screen_pos.x), static_cast<int>(imgui_cursor_screen_pos.y)};
    const ImageSize view_size{static_cast<int>(imgui_window_size.x), static_cast<int>(imgui_window_size.y)};

    imgui_text_outlined(UserInterface::Colors::white, UserInterface::Colors::black, fmt::format("video [{}x{}]", view_size.width, view_size.height));

    if (video_frame)
        imgui_text_outlined(UserInterface::Colors::white, UserInterface::Colors::black, fmt::format("{}", video_frame->print()));

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();

    return std::make_tuple(view_position, view_size);
}

#include "video_view.h"

#include <fmt/core.h>
#include <imgui.h>

#include "video_trimmer/ui/colors/colors.hpp"
#include "video_trimmer/ui/imgui_helpers.hpp"

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

void VideoView::render(sf::RenderWindow& window, VideoFrame* video_frame, const int finished_video_frame_queue_size, const int video_frame_scaler_queue_size)
{
    render_ui(video_frame, finished_video_frame_queue_size, video_frame_scaler_queue_size);
    render_content(window, video_frame);
}

void VideoView::render_content(sf::RenderWindow& window, VideoFrame* video_frame)
{
    if (video_frame) {
        // receive a new sprite texture
        texture_ = video_frame->texture()->release_texture();
        sprite_->setTexture(*texture_, true);
    }

    sprite_->setPosition(static_cast<float>(view_position_.x), static_cast<float>(view_position_.y));
    sprite_->setScale(static_cast<float>(view_size_.width) / static_cast<float>(texture_->getSize().x), static_cast<float>(view_size_.height) / static_cast<float>(texture_->getSize().y));

    window.draw(*sprite_);
}

void VideoView::render_ui(const VideoFrame* video_frame, const int finished_video_frame_queue_size, const int video_frame_scaler_queue_size)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("video");

    // current view position and size
    const ImVec2 imgui_cursor_screen_pos = ImGui::GetCursorScreenPos();
    const ImVec2 imgui_window_size = ImGui::GetWindowSize();

    view_position_ = ImagePosition{static_cast<int>(imgui_cursor_screen_pos.x), static_cast<int>(imgui_cursor_screen_pos.y)};
    view_size_ = ImageSize{static_cast<int>(imgui_window_size.x), static_cast<int>(imgui_window_size.y)};

    video_trimmer::ui::imgui_text_outlined(video_trimmer::ui::colors::white, video_trimmer::ui::colors::black, fmt::format("video [{}x{}]", view_size_.width, view_size_.height));
    video_trimmer::ui::imgui_text_outlined(video_trimmer::ui::colors::white, video_trimmer::ui::colors::black, fmt::format("video frame scaler queue: {}, finished video frames queue: {}", video_frame_scaler_queue_size, finished_video_frame_queue_size));

    if (video_frame)
        video_trimmer::ui::imgui_text_outlined(video_trimmer::ui::colors::white, video_trimmer::ui::colors::black, fmt::format("{}", video_frame->print()));

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

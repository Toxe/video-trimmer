#include "window.h"

#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <imgui.h>

#include "command_line/command_line.h"
#include "video/video_content_provider.hpp"

Window::Window(const CommandLine& cli)
    : window_video_mode_{cli.default_window_video_mode()}
{
    // create window
    spdlog::info("init window mode {}x{}", cli.video_mode().width, cli.video_mode().height);

    window_ = std::make_unique<sf::RenderWindow>(cli.video_mode(), title_, sf::Style::Default);
    window_->setVerticalSyncEnabled(true);
    window_->requestFocus();

    // create the video view
    video_view_ = std::make_unique<VideoView>();

    // init ImGui & ImGui-SFML and load a custom font
    ImGui::SFML::Init(*window_, false);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata-SemiBold.ttf", static_cast<float>(cli.font_size()));
    ImGui::SFML::UpdateFontTexture();
}

[[nodiscard]] ImageSize Window::size() const
{
    const auto window_size = window_->getSize();
    return {static_cast<int>(window_size.x), static_cast<int>(window_size.y)};
}

void Window::next_frame(const Duration elapsed_time)
{
    ImGui::SFML::Update(*window_, sf::microseconds(elapsed_time.as_microseconds()));
}

void Window::render(ImagePosition video_view_position, ImageSize video_view_size, VideoContentProvider& video_content_provider)
{
    window_->clear();

    {
        std::lock_guard<std::mutex> lock(mtx_);
        video_view_->render(*window_, video_view_position, video_view_size, video_content_provider);
    }

    ImGui::SFML::Render(*window_);
    window_->display();
}

void Window::close()
{
    if (window_->isOpen())
        window_->close();

    ImGui::SFML::Shutdown();
}

void Window::resized_window()
{
    const auto size = window_->getSize();
    adjust_view_to_window_size();

    spdlog::info("resized window to {}x{}", size.x, size.y);
}

void Window::adjust_view_to_window_size()
{
    const auto size = window_->getSize();
    sf::FloatRect visibleArea(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));
    window_->setView(sf::View(visibleArea));
}

#include "main_window.hpp"

#include <fmt/core.h>
#include <imgui-SFML.h>
#include <imgui.h>

#include "command_line/command_line.h"
#include "logger/logger.hpp"

MainWindow::MainWindow(const video_trimmer::CommandLine& cli)
    : window_video_mode_{cli.default_window_video_mode()}
{
    // create main_window
    video_trimmer::logger::log_info(fmt::format("init main_window mode {}x{}", cli.video_mode().width, cli.video_mode().height));

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

[[nodiscard]] ImageSize MainWindow::size() const
{
    const auto window_size = window_->getSize();
    return {static_cast<int>(window_size.x), static_cast<int>(window_size.y)};
}

void MainWindow::next_frame(const Duration elapsed_time)
{
    ImGui::SFML::Update(*window_, sf::microseconds(elapsed_time.as_microseconds()));
}

void MainWindow::render(const int finished_video_frame_queue_size, const int video_frame_scaler_queue_size, VideoFrame* video_frame)
{
    window_->clear();

    {
        std::lock_guard<std::mutex> lock(mtx_);
        video_view_->render(*window_, video_frame, finished_video_frame_queue_size, video_frame_scaler_queue_size);
    }

    ImGui::SFML::Render(*window_);
    window_->display();
}

void MainWindow::close()
{
    if (window_->isOpen())
        window_->close();

    ImGui::SFML::Shutdown();
}

void MainWindow::resized_window()
{
    const auto size = window_->getSize();
    adjust_view_to_window_size();

    video_trimmer::logger::log_info(fmt::format("resized main_window to {}x{}", size.x, size.y));
}

void MainWindow::adjust_view_to_window_size()
{
    const auto size = window_->getSize();
    sf::FloatRect visibleArea(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));
    window_->setView(sf::View(visibleArea));
}

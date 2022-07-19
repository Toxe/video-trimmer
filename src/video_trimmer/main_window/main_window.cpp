#include "main_window.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::main_window {

MainWindow::MainWindow(const video_trimmer::command_line::CommandLine& cli)
    : graphics_(std::make_unique<video_trimmer::graphics::Graphics>())
{
    graphics_->init_sdl();
    graphics_->create_window(title_, {cli.window_width(), cli.window_height()});
    graphics_->create_renderer(cli.disable_vsync());
    graphics_->init_imgui(cli.font_size());

    video_view_ = std::make_unique<video_trimmer::views::video_view::VideoView>();
}

void MainWindow::begin_frame()
{
    graphics_->begin_frame();
}

void MainWindow::render()
{
    video_view_->render(graphics_.get());
    graphics_->finish_frame();
}

void MainWindow::show_video_frame(std::unique_ptr<video_reader::frame::Frame> video_frame)
{
    video_view_->show_video_frame(graphics_.get(), std::move(video_frame));
}

void MainWindow::close()
{
    graphics_->shutdown_imgui();
    graphics_->shutdown_sdl();
}

void MainWindow::resized_window()
{
    const auto window_size = size();
    adjust_view_to_window_size();

    video_trimmer::logger::log_info(fmt::format("resized main_window to {}x{}", window_size.width, window_size.height));
}

void MainWindow::adjust_view_to_window_size()
{
    const auto current_size = size();

    // sf::FloatRect visibleArea(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));
    // window_->setView(sf::View(visibleArea));
}

}  // namespace video_trimmer::main_window

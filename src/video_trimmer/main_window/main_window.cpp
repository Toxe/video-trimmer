#include "main_window.hpp"

#include "fmt/core.h"

#include "../logger/logger.hpp"

namespace video_trimmer::main_window {

MainWindow::MainWindow(const command_line::CommandLine& cli)
    : graphics_(std::make_unique<graphics::Graphics>())
{
    graphics_->init_sdl();
    graphics_->create_window(title_, cli.window_size());
    graphics_->create_renderer(cli.disable_vsync());
    graphics_->init_imgui(cli.font_size());

    video_view_ = std::make_unique<views::video_view::VideoView>();
}

void MainWindow::begin_frame()
{
    graphics_->begin_frame();
}

clock::Duration MainWindow::render(const clock::Clock& frame_time_clock)
{
    video_view_->render(*graphics_);
    return graphics_->finish_frame(frame_time_clock);
}

void MainWindow::show_video_frame(std::unique_ptr<video_file::Frame> video_frame)
{
    video_view_->show_video_frame(*graphics_, std::move(video_frame));
}

void MainWindow::close()
{
    graphics_->shutdown_imgui();
    graphics_->shutdown_sdl();
}

void MainWindow::resized_window()
{
    const auto window_size = size();

    logger::log_info(fmt::format("resized main_window to {}x{}", window_size.width, window_size.height));
}

}  // namespace video_trimmer::main_window

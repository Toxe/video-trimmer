#pragma once

#include <memory>

#include "video_trimmer/command_line/command_line.hpp"
#include "video_trimmer/graphics/graphics.hpp"
#include "video_trimmer/include/types.hpp"
#include "video_trimmer/video_reader/frame/frame.hpp"
#include "video_trimmer/views/video_view/video_view.hpp"

namespace video_trimmer::main_window {

class MainWindow {
public:
    explicit MainWindow(const video_trimmer::command_line::CommandLine& cli);

    [[nodiscard]] video_trimmer::views::video_view::VideoView& video_view() const { return *video_view_; };

    [[nodiscard]] bool is_open() const { return graphics_->window_is_open(); };
    [[nodiscard]] Size size() const { return graphics_->window_size(); };

    void begin_frame();
    void render();

    void show_video_frame(std::unique_ptr<video_reader::frame::Frame> video_frame);

    void resized_window();
    void close();

private:
    const char* title_ = "Video Trimmer";

    std::unique_ptr<video_trimmer::graphics::Graphics> graphics_;
    std::unique_ptr<video_trimmer::views::video_view::VideoView> video_view_;

    void adjust_view_to_window_size();
};

}  // namespace video_trimmer::main_window

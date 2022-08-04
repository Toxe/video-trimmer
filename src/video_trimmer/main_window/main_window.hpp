#pragma once

#include <memory>

#include "types.hpp"
#include "video_trimmer/command_line/command_line.hpp"
#include "video_trimmer/graphics/graphics.hpp"
#include "video_trimmer/video_file/frame.hpp"
#include "video_trimmer/views/video_view/video_view.hpp"

namespace video_trimmer::main_window {

class MainWindow {
public:
    explicit MainWindow(const command_line::CommandLine& cli);

    [[nodiscard]] graphics::Graphics& graphics() const { return *graphics_; };
    [[nodiscard]] views::video_view::VideoView& video_view() const { return *video_view_; };

    [[nodiscard]] bool is_open() const { return graphics_->window_is_open(); };
    [[nodiscard]] Size size() const { return graphics_->window_size(); };

    void begin_frame();
    clock::Duration render(const clock::Clock& frame_time_clock);

    void show_video_frame(std::unique_ptr<video_file::Frame> video_frame);

    void resized_window();
    void close();

private:
    const char* title_ = "Video Trimmer";

    std::unique_ptr<graphics::Graphics> graphics_;
    std::unique_ptr<views::video_view::VideoView> video_view_;
};

}  // namespace video_trimmer::main_window

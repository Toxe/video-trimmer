#pragma once

#include <memory>
#include <mutex>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/VideoMode.hpp"

#include "types.h"
#include "video_trimmer/clock/duration.hpp"
#include "video_trimmer/command_line/command_line.h"
#include "video_trimmer/views/video_view/video_view.h"

namespace video_trimmer::main_window {

class MainWindow {
    const char* title_ = "Video Trimmer";

    sf::VideoMode window_video_mode_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<video_trimmer::views::video_view::VideoView> video_view_;

    std::mutex mtx_;

    void adjust_view_to_window_size();

public:
    explicit MainWindow(const video_trimmer::command_line::CommandLine& cli);

    [[nodiscard]] sf::RenderWindow& window() const { return *window_; };
    [[nodiscard]] video_trimmer::views::video_view::VideoView& video_view() const { return *video_view_; };

    [[nodiscard]] bool is_open() const { return window_->isOpen(); };

    [[nodiscard]] ImageSize size() const;

    void next_frame(video_trimmer::clock::Duration elapsed_time);
    void render(VideoFrame* video_frame);

    void resized_window();
    void close();
};

}  // namespace video_trimmer::main_window

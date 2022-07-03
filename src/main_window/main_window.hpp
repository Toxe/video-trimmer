#pragma once

#include <memory>
#include <mutex>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "clock/duration.h"
#include "command_line/command_line.h"
#include "types.h"
#include "views/video_view/video_view.h"

class VideoFrame;

class MainWindow {
    const char* title_ = "Video Trimmer";

    sf::VideoMode window_video_mode_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<VideoView> video_view_;

    std::mutex mtx_;

    void adjust_view_to_window_size();

public:
    MainWindow(const video_trimmer::CommandLine& cli);

    [[nodiscard]] sf::RenderWindow& window() const { return *window_; };
    [[nodiscard]] VideoView& video_view() const { return *video_view_; };

    [[nodiscard]] bool is_open() const { return window_->isOpen(); };

    [[nodiscard]] ImageSize size() const;

    void next_frame(Duration elapsed_time);
    void render(int finished_video_frame_queue_size, int video_frame_scaler_queue_size, VideoFrame* video_frame);

    void resized_window();
    void close();
};

#pragma once

#include <memory>
#include <mutex>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "clock/duration.h"
#include "types.h"
#include "video_view/video_view.h"

class CommandLine;
class VideoStream;

class Window {
    const char* title_ = "Video Trimmer";

    sf::VideoMode window_video_mode_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<VideoView> video_view_;

    std::mutex mtx_;

    void adjust_view_to_window_size();

public:
    Window(const CommandLine& cli);

    [[nodiscard]] sf::RenderWindow& window() const { return *window_; };
    [[nodiscard]] bool is_open() const { return window_->isOpen(); };

    [[nodiscard]] ImageSize size() const;

    void next_frame(const Duration elapsed_time);
    void render(ImagePosition video_view_position, ImageSize video_view_size, VideoStream& video_stream);

    void resized_window();
    void close();
};

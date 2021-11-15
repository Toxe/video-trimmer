#pragma once

#include <optional>
#include <string>

#include <SFML/Window/VideoMode.hpp>

class CommandLine {
    int font_size_;
    int window_width_;
    int window_height_;
    sf::VideoMode video_mode_;
    sf::VideoMode default_window_video_mode_;
    std::string video_filename_;

    [[nodiscard]] sf::VideoMode default_video_mode() const;
    [[nodiscard]] int default_font_size() const;

public:
    CommandLine(int argc, char* argv[]);

    [[nodiscard]] int font_size() const { return font_size_; }
    [[nodiscard]] sf::VideoMode video_mode() const { return video_mode_; };
    [[nodiscard]] sf::VideoMode default_window_video_mode() const { return default_window_video_mode_; };
    [[nodiscard]] const std::string& video_filename() const { return video_filename_; }
};

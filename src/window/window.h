#pragma once

#include <memory>
#include <mutex>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "types.h"
#include "clock/duration.h"

class CommandLine;

class Window {
    const char* title_ = "Video Trimmer";

    sf::VideoMode window_video_mode_;

    std::unique_ptr<sf::RenderWindow> window_;
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

    std::mutex mtx_;

    void adjust_view_to_window_size();

public:
    Window(const CommandLine& cli);

    [[nodiscard]] sf::RenderWindow& window() const { return *window_; };
    [[nodiscard]] ImageSize texture_size() const { return ImageSize{static_cast<int>(texture_->getSize().x), static_cast<int>(texture_->getSize().y)}; };
    [[nodiscard]] bool is_open() const { return window_->isOpen(); };

    [[nodiscard]] ImageSize size() const;

    void next_frame(const Duration elapsed_time);
    void render();

    void resized_window();
    void close();

    void resize_texture(const ImageSize image_size, const sf::Color* pixels);
};

#include "window.h"

#include <spdlog/spdlog.h>
#include <imgui-SFML.h>
#include <imgui.h>

#include "command_line/command_line.h"

Window::Window(const CommandLine& cli)
    : window_video_mode_{cli.default_window_video_mode()}
{
    // create window
    spdlog::info("init window mode {}x{}", cli.video_mode().width, cli.video_mode().height);

    window_ = std::make_unique<sf::RenderWindow>(cli.video_mode(), title_, sf::Style::Default);
    window_->setVerticalSyncEnabled(true);
    window_->requestFocus();

    // create render texture and sprite that shows the image
    sf::Image image;
    image.create(cli.video_mode().width, cli.video_mode().height);

    texture_ = std::make_unique<sf::Texture>();
    texture_->loadFromImage(image);

    sprite_ = std::make_unique<sf::Sprite>();
    sprite_->setTexture(*texture_);

    // init ImGui & ImGui-SFML and load a custom font
    ImGui::SFML::Init(*window_, false);

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata-SemiBold.ttf", static_cast<float>(cli.font_size()));
    ImGui::SFML::UpdateFontTexture();
}

[[nodiscard]] ImageSize Window::size() const
{
    const auto window_size = window_->getSize();
    return {static_cast<int>(window_size.x), static_cast<int>(window_size.y)};
}

void Window::next_frame(const Duration elapsed_time)
{
    ImGui::SFML::Update(*window_, sf::microseconds(elapsed_time.as_microseconds()));
}

void Window::render()
{
    window_->clear();

    {
        std::lock_guard<std::mutex> lock(mtx_);
        window_->draw(*sprite_);
    }

    ImGui::SFML::Render(*window_);
    window_->display();
}

void Window::close()
{
    if (window_->isOpen())
        window_->close();

    ImGui::SFML::Shutdown();
}

void Window::resized_window()
{
    const auto size = window_->getSize();
    adjust_view_to_window_size();

    spdlog::info("resized window to {}x{}", size.x, size.y);
}

void Window::adjust_view_to_window_size()
{
    const auto size = window_->getSize();
    sf::FloatRect visibleArea(0.0f, 0.0f, static_cast<float>(size.x), static_cast<float>(size.y));
    window_->setView(sf::View(visibleArea));
}

void Window::resize_texture(const ImageSize image_size, const sf::Color* pixels)
{
    std::lock_guard<std::mutex> lock(mtx_);

    texture_->create(image_size.width, image_size.height);
    texture_->update(reinterpret_cast<const sf::Uint8*>(pixels), static_cast<unsigned int>(image_size.width), static_cast<unsigned int>(image_size.height), 0, 0);

    sprite_->setTextureRect(sf::IntRect{0, 0, image_size.width, image_size.height});
}

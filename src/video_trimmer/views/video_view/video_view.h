#pragma once

#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "types.h"
#include "video_trimmer/video_content_provider/video_frame/video_frame.hpp"

namespace video_trimmer::views::video_view {

class VideoView {
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

    ImagePosition view_position_{};
    ImageSize view_size_{};

    void render_ui(const VideoFrame* video_frame);
    void render_content(sf::RenderWindow& window, VideoFrame* video_frame);

public:
    VideoView();

    void render(sf::RenderWindow& window, VideoFrame* video_frame);

    [[nodiscard]] ImageSize size() const { return view_size_; }
};

}  // namespace video_trimmer::views::video_view

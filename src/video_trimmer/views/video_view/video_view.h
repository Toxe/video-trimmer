#pragma once

#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "types.h"
#include "video_trimmer/video_stream_reader/video_frame/video_frame.hpp"

namespace video_trimmer::views::video_view {

class VideoView {
    std::unique_ptr<sf::Texture> texture_;
    std::unique_ptr<sf::Sprite> sprite_;

    ImagePosition view_position_{};
    ImageSize view_size_{};

    void render_ui(const VideoFrame* video_frame, int finished_video_frame_queue_size, int video_frame_scaler_queue_size);
    void render_content(sf::RenderWindow& window, VideoFrame* video_frame);

public:
    VideoView();

    void render(sf::RenderWindow& window, VideoFrame* video_frame, int finished_video_frame_queue_size, int video_frame_scaler_queue_size);

    [[nodiscard]] ImageSize size() const { return view_size_; }
};

}  // namespace video_trimmer::views::video_view

#pragma once

#include <memory>

#include "SFML/Graphics/Texture.hpp"

#include "../frame/frame.hpp"

namespace video_trimmer::video_content_provider::video_frame {

class Texture {
    std::unique_ptr<sf::Texture> texture_;

public:
    explicit Texture(frame::Frame* frame);

    [[nodiscard]] std::unique_ptr<sf::Texture> release_texture();
};

}  // namespace video_trimmer::video_content_provider::video_frame

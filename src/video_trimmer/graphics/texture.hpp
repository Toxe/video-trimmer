#pragma once

#include <memory>

#include "types.hpp"
#include "video_trimmer/video_reader/frame/frame.hpp"

namespace video_trimmer::graphics {

class Graphics;

class Texture {
public:
    Texture(Graphics* graphics, const video_reader::frame::Frame* video_frame);
    ~Texture();

    [[nodiscard]] ImageSize size() const;

    [[nodiscard]] bool is_compatible_with_video_frame(const video_reader::frame::Frame* video_frame) const;

    void update(video_reader::frame::Frame* video_frame);
    void draw(Graphics* graphics, ImagePosition dst_position, ImageSize dst_size);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

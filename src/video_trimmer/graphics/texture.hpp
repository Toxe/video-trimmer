#pragma once

#include <memory>

#include "../video_file/frame.hpp"
#include "types.hpp"

namespace video_trimmer::graphics {

class Graphics;

class Texture {
public:
    Texture(Graphics& graphics, Size size, video_file::PixelFormat pixel_format);
    ~Texture();

    [[nodiscard]] Size size() const;

    [[nodiscard]] bool is_compatible(Size size, video_file::PixelFormat pixel_format) const;

    void update(video_file::Frame* video_frame);
    void draw(Graphics& graphics, Position dst_position, Size dst_size);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

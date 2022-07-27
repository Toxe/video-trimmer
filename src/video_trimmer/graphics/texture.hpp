#pragma once

#include <memory>

#include "types.hpp"
#include "video_trimmer/video_file/frame.hpp"

namespace video_trimmer::graphics {

class Graphics;

class Texture {
public:
    Texture(Graphics* graphics, const video_file::Frame* video_frame);
    ~Texture();

    [[nodiscard]] Size size() const;

    [[nodiscard]] bool is_compatible_with_video_frame(const video_file::Frame* video_frame) const;

    void update(video_file::Frame* video_frame);
    void draw(Graphics* graphics, Position dst_position, Size dst_size);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

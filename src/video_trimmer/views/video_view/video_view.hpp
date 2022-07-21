#pragma once

#include <memory>

#include "video_trimmer/graphics/graphics.hpp"
#include "video_trimmer/graphics/texture.hpp"
#include "video_trimmer/include/types.hpp"
#include "video_trimmer/video_reader/frame/frame.hpp"

namespace video_trimmer::views::video_view {

class VideoView {
public:
    void render(graphics::Graphics* graphics);

    void show_video_frame(video_trimmer::graphics::Graphics* graphics, std::unique_ptr<video_reader::frame::Frame> video_frame);

    [[nodiscard]] Size size() const { return view_size_; }

private:
    std::unique_ptr<video_reader::frame::Frame> current_video_frame_;
    std::unique_ptr<graphics::Texture> texture_;

    Position view_position_{};
    Size view_size_{};

    void render_ui();
    void render_content(graphics::Graphics* graphics);
};

}  // namespace video_trimmer::views::video_view

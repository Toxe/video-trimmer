#pragma once

#include <memory>
#include <string>

#include "../frame/frame.hpp"

namespace video_trimmer::video_content_provider::video_frame {

class VideoFrame {
public:
    VideoFrame(std::unique_ptr<frame::Frame> frame);

    [[nodiscard]] std::string print() const;

    [[nodiscard]] int width() const { return frame_->dst_width(); };
    [[nodiscard]] int height() const { return frame_->dst_height(); };
    [[nodiscard]] double timestamp() const { return frame_->timestamp(); };

    [[nodiscard]] frame::Frame* frame() { return frame_.get(); }

private:
    std::unique_ptr<frame::Frame> frame_;
};

}  // namespace video_trimmer::video_content_provider::video_frame
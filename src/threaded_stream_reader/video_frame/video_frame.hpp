#pragma once

#include <memory>
#include <string>

#include "../adapters/frame/frame.hpp"

class VideoFrame {
    std::unique_ptr<Frame> frame_;

public:
    VideoFrame(std::unique_ptr<Frame> frame);

    [[nodiscard]] std::string print() const;

    [[nodiscard]] int width() const { return frame_->dst_width(); };
    [[nodiscard]] int height() const { return frame_->dst_height(); };
    [[nodiscard]] double timestamp() const { return frame_->timestamp(); };

    [[nodiscard]] Frame* frame() { return frame_.get(); }
};

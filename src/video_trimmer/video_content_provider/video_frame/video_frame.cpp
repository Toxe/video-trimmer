#include "video_frame.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_content_provider::video_frame {

VideoFrame::VideoFrame(std::unique_ptr<frame::Frame> frame)
    : frame_{std::move(frame)}
{
}

std::string VideoFrame::print() const
{
    return fmt::format("[VideoFrame {:.4f}, {}x{}]", timestamp(), width(), height());
}

void VideoFrame::create_texture()
{
    texture_ = std::make_unique<Texture>(frame_.get());
}

}  // namespace video_trimmer::video_content_provider::video_frame

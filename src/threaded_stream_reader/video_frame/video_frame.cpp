#include "video_frame.hpp"

#include <fmt/core.h>

VideoFrame::VideoFrame(std::unique_ptr<Frame> frame)
    : frame_{std::move(frame)}
{
}

std::string VideoFrame::print() const
{
    return fmt::format("[VideoFrame {:.4f}, {}x{}]", timestamp(), width(), height());
}

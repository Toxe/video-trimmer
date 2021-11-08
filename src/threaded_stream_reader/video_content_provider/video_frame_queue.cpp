#include "video_frame_queue.hpp"

#include <algorithm>

#include "../video_frame/video_frame.hpp"

void VideoFrameQueue::push(std::unique_ptr<VideoFrame> video_frame)
{
    queue_.push_back(std::move(video_frame));
    std::sort(queue_.begin(), queue_.end(), [](const std::unique_ptr<VideoFrame>& left, const std::unique_ptr<VideoFrame>& right) { return left->timestamp() < right->timestamp(); });
}

std::unique_ptr<VideoFrame> VideoFrameQueue::pop(double playback_position)
{
    std::unique_ptr<VideoFrame> video_frame = nullptr;

    if (!queue_.empty() && queue_.front()->timestamp() <= playback_position) {
        video_frame = std::move(queue_.front());
        queue_.erase(queue_.begin());
    }

    return video_frame;
}

int VideoFrameQueue::size() const
{
    return static_cast<int>(queue_.size());
}

bool VideoFrameQueue::empty() const
{
    return queue_.empty();
}

bool VideoFrameQueue::full() const
{
    return queue_.size() >= max_queue_size_;
}

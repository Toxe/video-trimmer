#pragma once

#include <memory>
#include <vector>

class VideoFrame;

class VideoFrameQueue {
    const std::size_t max_queue_size_ = 10;
    std::vector<std::unique_ptr<VideoFrame>> queue_;

public:
    void push(std::unique_ptr<VideoFrame> video_frame);
    [[nodiscard]] std::unique_ptr<VideoFrame> pop(double playback_position);

    [[nodiscard]] int size() const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] bool full() const;
};

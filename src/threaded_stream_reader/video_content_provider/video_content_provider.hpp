#pragma once

#include <memory>
#include <mutex>
#include <tuple>

#include "../video_file.hpp"
#include "video_frame_queue.hpp"
#include "video_frame_scaler.hpp"
#include "video_reader.hpp"

class Factory;
class VideoFrame;

class VideoContentProvider {
    std::mutex mtx_;

    Factory* factory_;

    VideoFrameQueue finished_video_frames_queue_;
    VideoFrameScaler video_frame_scaler_;
    VideoReader video_reader_;

    bool is_running_ = false;

public:
    VideoContentProvider(Factory* factory, VideoFile& video_file, const int scale_width, const int scale_height);
    ~VideoContentProvider();

    void run();
    void stop();

    bool has_finished();
    bool video_reader_has_finished();

    bool finished_video_frames_queue_is_not_full();

    void add_video_frame_for_scaling(std::unique_ptr<VideoFrame> video_frame);
    void add_finished_video_frame(std::unique_ptr<VideoFrame> video_frame);

    [[nodiscard]] std::tuple<std::unique_ptr<VideoFrame>, int> next_frame(const double playback_position);

    void change_scaling_dimensions(const int scale_width, const int scale_height);
};

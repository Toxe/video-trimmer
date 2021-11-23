#pragma once

#include <chrono>
#include <memory>

#include "threaded_stream_reader/factory/ffmpeg_factory.hpp"
#include "threaded_stream_reader/video_content_provider/video_content_provider.hpp"
#include "types.h"

class VideoFile;
class VideoFrame;

class VideoPlayer {
    std::unique_ptr<FFmpegFactory> factory_;
    std::unique_ptr<VideoFile> video_file_;
    std::unique_ptr<VideoContentProvider> video_content_provider_;

    std::chrono::steady_clock::time_point time_point_playback_start_;
    std::chrono::duration<double> playback_position_;
    bool received_first_real_frame_ = false;

    bool is_playing_ = false;

public:
    VideoPlayer();

    bool open_file(const char* filename);
    void close_file();

    void start();
    void stop();

    [[nodiscard]] bool has_open_file();
    [[nodiscard]] bool is_playing();
    [[nodiscard]] bool is_stopped();

    void update();

    [[nodiscard]] std::unique_ptr<VideoFrame> next_frame();

    void change_scaling_dimensions(ImageSize image_size);

    [[nodiscard]] double playback_position();

    [[nodiscard]] int finished_video_frame_queue_size();
    [[nodiscard]] int video_frame_scaler_queue_size();

    [[nodiscard]] const VideoFile* video_file();
};

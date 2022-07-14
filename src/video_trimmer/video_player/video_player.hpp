#pragma once

#include <chrono>
#include <memory>

#include "types.h"
#include "video_trimmer/video_reader/video_file/video_file.hpp"
#include "video_trimmer/video_reader/video_reader.hpp"

namespace video_trimmer::video_player {

class VideoPlayer {
public:
    bool open_file(const char* filename);
    void close_file();

    void start();
    void resume();
    void pause();

    [[nodiscard]] bool has_open_file();
    [[nodiscard]] bool has_started_playing();
    [[nodiscard]] bool is_playing();
    [[nodiscard]] bool is_paused();

    void update();

    [[nodiscard]] std::unique_ptr<video_reader::frame::Frame> next_frame();

    void change_scaling_dimensions(ImageSize image_size);

    [[nodiscard]] double playback_position();

    [[nodiscard]] const video_reader::video_file::VideoFile* video_file();

    void render();

private:
    std::unique_ptr<video_reader::VideoReader> video_reader_;
    std::unique_ptr<video_reader::video_file::VideoFile> video_file_;

    std::chrono::steady_clock::time_point time_point_playback_start_;
    std::chrono::duration<double> playback_position_ = std::chrono::duration<double>::zero();

    bool has_started_playing_;
    bool is_playing_;
    bool received_first_real_frame_;
};

}  // namespace video_trimmer::video_player

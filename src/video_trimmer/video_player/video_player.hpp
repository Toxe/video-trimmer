#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "../video_file/video_file.hpp"
#include "types.hpp"

namespace video_trimmer::video_player {

class VideoPlayer {
public:
    explicit VideoPlayer(bool dump_first_video_frame);

    bool open_file(const std::string& filename);
    void close_file();

    void start();
    void stop();
    void toggle_pause();

    void jump_backward();
    void jump_forward();

    [[nodiscard]] bool has_open_file();
    [[nodiscard]] bool has_started_playing();
    [[nodiscard]] bool is_playing();
    [[nodiscard]] bool is_paused();

    [[nodiscard]] std::unique_ptr<video_file::Frame> next_frame();

    [[nodiscard]] double playback_position() const { return playback_position_; };

    [[nodiscard]] video_file::VideoFile* video_file() { return video_file_.get(); }

    [[nodiscard]] int received_frames() const { return received_frames_; }
    [[nodiscard]] int dropped_frames() const { return dropped_frames_; }

private:
    std::unique_ptr<video_file::Frame> available_frame_;
    std::unique_ptr<video_file::VideoFile> video_file_;

    std::chrono::steady_clock::time_point previous_frame_start_;
    std::chrono::steady_clock::time_point current_frame_start_;
    double playback_position_ = 0.0;

    bool has_started_playing_ = false;
    bool is_playing_ = false;
    bool received_first_real_frame_ = false;

    bool dump_first_frame_ = false;

    int received_frames_ = 0;
    int dropped_frames_ = 0;
};

}  // namespace video_trimmer::video_player

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

    void play_file(std::unique_ptr<video_file::VideoFile> video_file, std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> current_time);
    void close_file();

    void start();
    void stop();
    void toggle_pause();

    void jump_backward(std::chrono::seconds seconds);
    void jump_forward(std::chrono::seconds seconds);

    [[nodiscard]] bool has_open_file() const;
    [[nodiscard]] bool has_started_playing() const;
    [[nodiscard]] bool has_received_first_real_frame() const;
    [[nodiscard]] bool is_playing() const;
    [[nodiscard]] bool is_paused() const;
    [[nodiscard]] bool is_seeking() const;

    void update_time(std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> current_time);

    [[nodiscard]] std::chrono::microseconds playback_position() const { return playback_position_; };

    [[nodiscard]] video_file::VideoFile* video_file() const { return video_file_.get(); }

    [[nodiscard]] std::unique_ptr<video_file::Frame> next_frame(std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> current_time);

    [[nodiscard]] int received_frames() const { return received_frames_; }
    [[nodiscard]] int dropped_frames() const { return dropped_frames_; }

    void set_has_received_first_real_frame(bool flag) { has_received_first_real_frame_ = flag; };

private:
    std::unique_ptr<video_file::Frame> available_frame_;
    std::unique_ptr<video_file::VideoFile> video_file_;

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> previous_frame_start_;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> current_frame_start_;
    std::chrono::microseconds playback_position_ = std::chrono::microseconds{0};

    bool is_playing_ = false;
    bool is_seeking_ = false;
    bool has_started_playing_ = false;
    bool has_received_first_real_frame_ = false;
    bool was_seeking_ = false;

    bool dump_first_frame_ = false;

    int received_frames_ = 0;
    int dropped_frames_ = 0;

    [[nodiscard]] std::unique_ptr<video_file::Frame> next_frame_normal_playback();
    [[nodiscard]] std::unique_ptr<video_file::Frame> next_frame_seeking();

    void set_current_frame_start(std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> current_time);
};

}  // namespace video_trimmer::video_player

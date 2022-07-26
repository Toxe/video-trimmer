#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "types.hpp"
#include "video_trimmer/command_line/command_line.hpp"
#include "video_trimmer/video_reader/video_file/video_file.hpp"
#include "video_trimmer/video_reader/video_reader.hpp"

namespace video_trimmer::video_player {

class VideoPlayer {
public:
    explicit VideoPlayer(const video_trimmer::command_line::CommandLine& cli);

    bool open_file(const std::string& filename);
    void close_file();

    void start();
    void stop();
    void toggle_pause();

    [[nodiscard]] bool has_open_file();
    [[nodiscard]] bool has_started_playing();
    [[nodiscard]] bool is_playing();
    [[nodiscard]] bool is_paused();

    void update();

    [[nodiscard]] std::unique_ptr<video_reader::frame::Frame> next_frame();

    [[nodiscard]] double playback_position() const { return playback_position_; };

    [[nodiscard]] video_reader::video_file::VideoFile* video_file() { return video_file_.get(); }

    [[nodiscard]] int received_frames() const { return received_frames_; }
    [[nodiscard]] int dropped_frames() const { return dropped_frames_; }

private:
    std::unique_ptr<video_reader::frame::Frame> available_frame_;

    std::unique_ptr<video_reader::VideoReader> video_reader_;
    std::unique_ptr<video_reader::video_file::VideoFile> video_file_;

    std::chrono::steady_clock::time_point previous_frame_start_;
    std::chrono::steady_clock::time_point current_frame_start_;
    double playback_position_;

    bool has_started_playing_;
    bool is_playing_;
    bool received_first_real_frame_;

    bool dump_first_frame_ = false;

    int received_frames_ = 0;
    int dropped_frames_ = 0;
};

}  // namespace video_trimmer::video_player

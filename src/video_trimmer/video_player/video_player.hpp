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

    [[nodiscard]] double playback_position() { return playback_position_.count(); };

    [[nodiscard]] video_reader::video_file::VideoFile* video_file() { return video_file_.get(); }

private:
    std::unique_ptr<video_reader::VideoReader> video_reader_;
    std::unique_ptr<video_reader::video_file::VideoFile> video_file_;

    std::chrono::steady_clock::time_point time_point_playback_start_;
    std::chrono::duration<double> playback_position_ = std::chrono::duration<double>::zero();

    bool has_started_playing_;
    bool is_playing_;
    bool received_first_real_frame_;

    bool dump_first_frame_ = false;
};

}  // namespace video_trimmer::video_player

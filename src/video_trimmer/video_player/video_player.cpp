#include "video_player.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_player {

VideoPlayer::VideoPlayer(const command_line::CommandLine& cli)
    : dump_first_frame_(cli.dump_first_video_frame())
{
}

bool VideoPlayer::open_file(const std::string& filename)
{
    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) open file: {}", filename));

    if (has_open_file())
        return false;

    video_file_ = std::make_unique<video_reader::video_file::VideoFile>(filename);

    if (has_open_file())
        video_reader_ = std::make_unique<video_reader::VideoReader>(*video_file_);

    has_started_playing_ = false;
    is_playing_ = false;
    received_first_real_frame_ = false;
    time_point_playback_start_ = std::chrono::steady_clock::now();
    playback_position_ = std::chrono::duration<double>::zero();

    video_file_->set_dump_first_frame(dump_first_frame_);

    return has_open_file();
}

void VideoPlayer::close_file()
{
    if (is_playing())
        stop();

    video_file_ = nullptr;
}

void VideoPlayer::start()
{
    if (has_open_file() && !has_started_playing()) {
        has_started_playing_ = true;
        is_playing_ = true;
        received_first_real_frame_ = false;
        time_point_playback_start_ = std::chrono::steady_clock::now();
        playback_position_ = std::chrono::duration<double>::zero();
    }
}

void VideoPlayer::stop()
{
    if (is_playing())
        is_playing_ = false;
}

void VideoPlayer::toggle_pause()
{
    if (is_playing())
        is_playing_ = false;
    else if (is_paused())
        is_playing_ = true;
}

bool VideoPlayer::has_open_file()
{
    return video_file_ && video_file_->is_open();
}

bool VideoPlayer::has_started_playing()
{
    return has_open_file() && has_started_playing_;
}

bool VideoPlayer::is_playing()
{
    return has_started_playing() && is_playing_;
}

bool VideoPlayer::is_paused()
{
    return has_started_playing() && !is_playing_;
}

void VideoPlayer::update()
{
    if (is_playing()) {
        // current position in playback
        if (!received_first_real_frame_) {
            time_point_playback_start_ = std::chrono::steady_clock::now();
            playback_position_ = std::chrono::duration<double>::zero();
        } else {
            playback_position_ = std::chrono::steady_clock::now() - time_point_playback_start_;
        }
    }
}

std::unique_ptr<video_reader::frame::Frame> VideoPlayer::next_frame()
{
    if (!is_playing())
        return nullptr;

    auto video_frame = video_reader_->read_next_frame(playback_position_.count());

    if (!video_frame)
        return nullptr;

    if (!received_first_real_frame_) {
        video_trimmer::logger::log_debug("(VideoPlayer) received first frame, begin playback");
        received_first_real_frame_ = true;
    }

    return video_frame;
}

}  // namespace video_trimmer::video_player

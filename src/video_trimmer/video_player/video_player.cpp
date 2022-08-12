#include "video_player.hpp"

#include <cmath>

#include "fmt/chrono.h"
#include "fmt/core.h"

#include "../logger/logger.hpp"

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

    video_file_ = std::make_unique<video_file::VideoFile>(filename);

    has_started_playing_ = false;
    is_playing_ = false;
    received_first_real_frame_ = false;
    playback_position_ = 0.0;

    current_frame_start_ = std::chrono::steady_clock::now();
    previous_frame_start_ = current_frame_start_;

    received_frames_ = 0;
    dropped_frames_ = 0;

    available_frame_ = nullptr;

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
        playback_position_ = 0.0;

        current_frame_start_ = std::chrono::steady_clock::now();
        previous_frame_start_ = current_frame_start_;
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

void VideoPlayer::jump_backward()
{
    if (has_started_playing()) {
        double skip_to = std::max(playback_position_ - 10.0, 0.0);
        logger::log_trace(fmt::format("jump to {:.3f} - 10s = {:.3f}", playback_position_, skip_to));

        if (video_file_->seek_position(skip_to, -1)) {
            available_frame_ = nullptr;
            playback_position_ = skip_to;
            received_first_real_frame_ = false;

            current_frame_start_ = std::chrono::steady_clock::now();
            previous_frame_start_ = current_frame_start_;
        }
    }
}

void VideoPlayer::jump_forward()
{
    if (has_started_playing()) {
        double skip_to = std::min(playback_position_ + 10.0, static_cast<double>(video_file_->duration()));
        logger::log_trace(fmt::format("jump to {:.3f} + 10s = {:.3f}", playback_position_, skip_to));

        if (video_file_->seek_position(skip_to, 1)) {
            available_frame_ = nullptr;
            playback_position_ = skip_to;
            received_first_real_frame_ = false;

            current_frame_start_ = std::chrono::steady_clock::now();
            previous_frame_start_ = current_frame_start_;
        }
    }
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

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame()
{
    if (!has_started_playing())
        return nullptr;

    previous_frame_start_ = current_frame_start_;
    current_frame_start_ = std::chrono::steady_clock::now();

    if (!is_playing())
        return nullptr;

    const std::chrono::duration<double> diff = current_frame_start_ - previous_frame_start_;
    const double prev_playback_position = playback_position_;

    // current position in playback
    if (received_first_real_frame_)
        playback_position_ += diff.count();

    logger::log_trace(fmt::format("[next_frame A] {:.3f} --> {:.3f} ---------------------", prev_playback_position, playback_position_));

    if (!available_frame_) {
        logger::log_trace(fmt::format("[next_frame B] {:.3f} | no available_frame", playback_position_));

        while (true) {
            logger::log_trace(fmt::format("[next_frame C] {:.3f} | request new frame", playback_position_));

            available_frame_ = video_file_->read_next_frame();

            if (!available_frame_) {
                logger::log_trace(fmt::format("[next_frame D] {:.3f} | no new frame available", playback_position_));

                if (!received_first_real_frame_)
                    continue;  // still waiting for the first frame, so keep reading

                return nullptr;
            }

            ++received_frames_;

            if (available_frame_->timestamp() < prev_playback_position) {
                logger::log_trace(fmt::format("[next_frame E] {:.3f} | received frame {:.3f}, TOO OLD", playback_position_, available_frame_->timestamp()));
                ++dropped_frames_;
                continue;
            }

            break;
        }
    }

    if (!received_first_real_frame_) {
        logger::log_trace(fmt::format("[next_frame F] {:.3f} | received first frame, begin playback", playback_position_));
        received_first_real_frame_ = true;
        playback_position_ = available_frame_->timestamp();
        current_frame_start_ = std::chrono::steady_clock::now();
    }

    if (playback_position_ >= available_frame_->timestamp()) {
        logger::log_trace(fmt::format("[next_frame G] {:.3f} | available frame {:.3f}, RETURN", playback_position_, available_frame_->timestamp()));
        return std::move(available_frame_);
    }

    logger::log_trace(fmt::format("[next_frame H] {:.3f} | available frame {:.3f}, WAIT", playback_position_, available_frame_->timestamp()));
    return nullptr;
}

}  // namespace video_trimmer::video_player

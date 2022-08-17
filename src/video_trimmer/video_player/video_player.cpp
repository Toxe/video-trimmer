#include "video_player.hpp"

#include <cassert>
#include <cmath>

#include "fmt/core.h"

#include "../logger/logger.hpp"

namespace video_trimmer::video_player {

VideoPlayer::VideoPlayer(bool dump_first_video_frame)
    : dump_first_frame_(dump_first_video_frame)
{
}

void VideoPlayer::play_file(std::unique_ptr<video_file::VideoFile> video_file, std::chrono::steady_clock::time_point current_time)
{
    assert(video_file != nullptr);

    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) play file: {}", video_file->filename()));

    current_time_ = current_time;

    video_file_ = std::move(video_file);
    video_file_->set_dump_first_frame(dump_first_frame_);

    has_started_playing_ = false;
    is_playing_ = false;
    has_received_first_real_frame_ = false;
    playback_position_ = 0.0;

    current_frame_start_ = current_time_;
    previous_frame_start_ = current_frame_start_;

    received_frames_ = 0;
    dropped_frames_ = 0;

    available_frame_ = nullptr;
}

void VideoPlayer::close_file()
{
    if (is_playing())
        stop();

    video_file_ = nullptr;
}

void VideoPlayer::start(std::chrono::steady_clock::time_point current_time)
{
    if (has_open_file() && !has_started_playing()) {
        current_time_ = current_time;
        has_started_playing_ = true;
        is_playing_ = true;
        has_received_first_real_frame_ = false;
        playback_position_ = 0.0;

        current_frame_start_ = current_time_;
        previous_frame_start_ = current_frame_start_;
    }
}

void VideoPlayer::stop()
{
    if (is_playing())
        is_playing_ = false;

    if (is_seeking())
        is_seeking_ = false;
}

void VideoPlayer::toggle_pause()
{
    if (is_playing())
        is_playing_ = false;
    else if (is_paused())
        is_playing_ = true;
}

void VideoPlayer::jump_backward(std::chrono::steady_clock::time_point current_time)
{
    if (has_started_playing()) {
        double skip_to = std::max(playback_position_ - 10.0, 0.0);
        logger::log_trace(fmt::format("jump to {:.3f} - 10s = {:.3f}/{:.3f}", playback_position_, skip_to, video_file_->duration()));

        if (video_file_->seek_position(skip_to, -1)) {
            current_time_ = current_time;
            is_seeking_ = true;

            available_frame_ = nullptr;
            playback_position_ = skip_to;
            has_received_first_real_frame_ = false;

            current_frame_start_ = current_time_;
            previous_frame_start_ = current_frame_start_;
        }
    }
}

void VideoPlayer::jump_forward(std::chrono::steady_clock::time_point current_time)
{
    if (has_started_playing()) {
        double skip_to = std::min(playback_position_ + 10.0, static_cast<double>(video_file_->duration()));
        logger::log_trace(fmt::format("jump to {:.3f} + 10s = {:.3f}/{:.3f}", playback_position_, skip_to, video_file_->duration()));

        if (video_file_->seek_position(skip_to, -1)) {  // TODO: always -1?
            current_time_ = current_time;
            is_seeking_ = true;

            available_frame_ = nullptr;
            playback_position_ = skip_to;
            has_received_first_real_frame_ = false;

            current_frame_start_ = current_time_;
            previous_frame_start_ = current_frame_start_;
        }
    }
}

bool VideoPlayer::has_open_file() const
{
    return video_file_ && video_file_->is_open();
}

bool VideoPlayer::has_started_playing() const
{
    return has_open_file() && has_started_playing_;
}

bool VideoPlayer::has_received_first_real_frame() const
{
    return has_received_first_real_frame_;
}

bool VideoPlayer::is_playing() const
{
    return has_started_playing() && is_playing_;
}

bool VideoPlayer::is_paused() const
{
    return has_started_playing() && !is_playing_;
}

bool VideoPlayer::is_seeking() const
{
    return has_started_playing() && is_seeking_;
}

void VideoPlayer::update_time(std::chrono::steady_clock::time_point current_time)
{
    assert(current_time >= current_time_);

    current_time_ = current_time;

    if (!has_started_playing())
        return;

    previous_frame_start_ = current_frame_start_;
    current_frame_start_ = current_time;

    if (!is_playing())
        return;

    // current position in playback
    if (has_received_first_real_frame_) {
        const std::chrono::duration<double> diff = current_frame_start_ - previous_frame_start_;
        playback_position_ += diff.count();
    }
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame(std::chrono::steady_clock::time_point current_time)
{
    update_time(current_time);

    if (!has_started_playing())
        return nullptr;

    if (!is_playing())
        return nullptr;

    logger::log_trace(fmt::format("[next_frame] {:.3f} ---------------------", playback_position_));

    if (is_seeking_)
        return next_frame_seeking();
    else
        return next_frame_normal_playback();
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame_normal_playback()
{
    if (!available_frame_) {
        logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | no available frame", playback_position_));

        while (true) {
            logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | request new frame", playback_position_));

            available_frame_ = video_file_->read_next_frame();

            if (!available_frame_) {
                logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | no new frame available", playback_position_));

                if (!has_received_first_real_frame_)
                    continue;  // still waiting for the first frame, so keep reading

                if (playback_position_ >= video_file_->duration()) {
                    logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | reached the end --> STOP PLAYBACK", playback_position_));
                    stop();
                }

                return nullptr;
            }

            ++received_frames_;

            if ((available_frame_->timestamp() + available_frame_->duration()) < playback_position_) {
                logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | received {}, TOO OLD", playback_position_, available_frame_->description()));
                ++dropped_frames_;
                continue;
            }

            break;
        }
    }

    if (!has_received_first_real_frame_) {
        logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | received first frame, begin playback", playback_position_));
        has_received_first_real_frame_ = true;
        playback_position_ = available_frame_->timestamp();
        current_frame_start_ = current_time_;
    }

    if (playback_position_ >= available_frame_->timestamp()) {
        logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | available {}, RETURN", playback_position_, available_frame_->description()));
        return std::move(available_frame_);
    }

    logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | available {}, WAIT", playback_position_, available_frame_->description()));
    return nullptr;
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame_seeking()
{
    bool found = false;
    bool should_stop = false;

    while (true) {
        logger::log_trace(fmt::format("[next_frame_seeking] {:.3f} | request new frame", playback_position_));

        std::unique_ptr<video_file::Frame> frame = video_file_->read_next_frame();

        if (frame) {
            ++received_frames_;

            if ((frame->timestamp() + frame->duration()) < playback_position_) {
                available_frame_ = std::move(frame);
                ++dropped_frames_;

                logger::log_trace(fmt::format("[next_frame_seeking] {:.3f} | received {}, TOO OLD", playback_position_, available_frame_->description()));
                found = true;
                continue;
            } else {
                available_frame_ = std::move(frame);
                logger::log_trace(fmt::format("[next_frame_seeking] {:.3f} | received {}, BREAK", playback_position_, available_frame_->description()));
                break;
            }
        } else {
            logger::log_trace(fmt::format("[next_frame_seeking] {:.3f} | no new frame available", playback_position_));

            if (found) {
                logger::log_trace(fmt::format("[next_frame_seeking] {:.3f} | available {}, FOUND", playback_position_, available_frame_->description()));
                should_stop = true;
                break;
            }
        }
    }

    if (!has_received_first_real_frame_) {
        logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | received first frame, begin playback", playback_position_));
        has_received_first_real_frame_ = true;
        playback_position_ = available_frame_->timestamp();
        current_frame_start_ = current_time_;
    }

    is_seeking_ = false;

    if (should_stop)
        stop();

    if (playback_position_ >= available_frame_->timestamp()) {
        logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | available {}, RETURN", playback_position_, available_frame_->description()));
        return std::move(available_frame_);
    }

    logger::log_trace(fmt::format("[next_frame_normal_playback] {:.3f} | available {}, WAIT", playback_position_, available_frame_->description()));
    return nullptr;
}

}  // namespace video_trimmer::video_player

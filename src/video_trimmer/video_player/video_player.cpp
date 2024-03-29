#include "video_player.hpp"

#include <cassert>
#include <cmath>

#include "fmt/core.h"

#include "../clock/fmt.hpp"
#include "../logger/logger.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::video_player {

VideoPlayer::VideoPlayer(bool dump_first_video_frame)
    : dump_first_frame_(dump_first_video_frame)
{
}

void VideoPlayer::play_file(std::unique_ptr<video_file::VideoFile> video_file)
{
    assert(video_file != nullptr);

    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) play file: {}", video_file->filename()));

    video_file_ = std::move(video_file);
    video_file_->set_dump_first_frame(dump_first_frame_);

    has_started_playing_ = false;
    is_playing_ = false;
    has_received_first_real_frame_ = false;
    playback_position_ = 0us;

    clock_ = clock::PlaybackClock();

    set_current_frame_start(clock_.now());
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

void VideoPlayer::start()
{
    if (has_open_file() && !has_started_playing()) {
        has_started_playing_ = true;
        is_playing_ = true;
        has_received_first_real_frame_ = false;
        playback_position_ = 0us;
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

void VideoPlayer::jump_backward(const std::chrono::seconds seconds)
{
    if (has_started_playing()) {
        const clock::PlaybackPosition skip_to = std::max(playback_position_ - seconds, clock::PlaybackPosition{0us});
        logger::log_trace(fmt::format("jump to {} - {} = {}/{}", playback_position_, seconds, skip_to, video_file_->duration()));

        if (video_file_->seek_position(skip_to)) {
            is_seeking_ = true;

            available_frame_ = nullptr;
            playback_position_ = skip_to;
            has_received_first_real_frame_ = false;
        }
    }
}

void VideoPlayer::jump_forward(const std::chrono::seconds seconds)
{
    if (has_started_playing()) {
        const clock::PlaybackPosition skip_to = std::min(playback_position_ + seconds, clock::PlaybackPosition{video_file_->duration()});
        logger::log_trace(fmt::format("jump to {} + {} = {}/{}", playback_position_, seconds, skip_to, video_file_->duration()));

        if (video_file_->seek_position(skip_to)) {
            is_seeking_ = true;

            available_frame_ = nullptr;
            playback_position_ = skip_to;
            has_received_first_real_frame_ = false;
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

void VideoPlayer::set_current_frame_start(const clock::PlaybackTimePoint current_time)
{
    if (is_playing())
        logger::log_trace(fmt::format("current frame start: {} +{} --> {}", current_frame_start_, current_time - current_frame_start_, current_time));

    current_frame_start_ = current_time;
}

void VideoPlayer::update_time(const clock::PlaybackTimePoint current_time)
{
    if (!has_started_playing())
        return;

    previous_frame_start_ = current_frame_start_;
    set_current_frame_start(current_time);

    if (!is_playing())
        return;

    // update current position in playback
    if (has_received_first_real_frame_ && !was_seeking_)
        playback_position_ = playback_position_ + (current_frame_start_ - previous_frame_start_);

    was_seeking_ = false;
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame(const clock::PlaybackTimePoint current_time)
{
    update_time(current_time);

    if (!has_started_playing())
        return nullptr;

    if (is_paused() && !is_seeking())
        return nullptr;

    logger::log_trace(fmt::format("[next_frame] playback position: {} / {}", playback_position_, video_file_->duration()));

    if (is_seeking_)
        return next_frame_seeking();
    else
        return next_frame_normal_playback();
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame_normal_playback()
{
    const char* function_name = "next_frame_normal_playback";

    if (!available_frame_) {
        logger::log_trace(fmt::format("[{}] {} -- no available frame", function_name, playback_position_));

        while (true) {
            logger::log_trace(fmt::format("[{}] {} -- request new frame", function_name, playback_position_));

            available_frame_ = video_file_->read_next_frame();

            if (!available_frame_) {
                logger::log_trace(fmt::format("[{}] {} -- no new frame available", function_name, playback_position_));

                if (!has_received_first_real_frame_)
                    continue;  // still waiting for the first frame, so keep reading

                if (playback_position_ >= video_file_->duration()) {
                    logger::log_trace(fmt::format("[{}] {} -- reached the end --> STOP PLAYBACK", function_name, playback_position_));
                    stop();
                }

                return nullptr;
            }

            ++received_frames_;

            if ((available_frame_->timestamp() + available_frame_->duration()) < playback_position_) {
                logger::log_trace(fmt::format("[{}] {} -- received {}, TOO OLD", function_name, playback_position_, available_frame_->description()));
                ++dropped_frames_;
                continue;
            }

            break;
        }
    }

    if (!has_received_first_real_frame_) {
        logger::log_trace(fmt::format("[{}] {} -- received first frame, begin playback", function_name, playback_position_));
        has_received_first_real_frame_ = true;
        playback_position_ = available_frame_->timestamp();
        // set_current_frame_start(std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()));  // TODO
    }

    if (playback_position_ >= available_frame_->timestamp()) {
        logger::log_trace(fmt::format("[{}] {} -- available {}, RETURN", function_name, playback_position_, available_frame_->description()));
        return std::move(available_frame_);
    }

    logger::log_trace(fmt::format("[{}] {} -- available {}, WAIT", function_name, playback_position_, available_frame_->description()));
    return nullptr;
}

std::unique_ptr<video_file::Frame> VideoPlayer::next_frame_seeking()
{
    const char* function_name = "next_frame_seeking";

    bool found = false;
    bool should_stop = false;

    while (true) {
        logger::log_trace(fmt::format("[{}] {} -- request new frame", function_name, playback_position_));

        std::unique_ptr<video_file::Frame> frame = video_file_->read_next_frame();

        if (frame) {
            ++received_frames_;

            if ((frame->timestamp() + frame->duration()) < playback_position_) {
                available_frame_ = std::move(frame);

                logger::log_trace(fmt::format("[{}] {} -- received {}, TOO OLD", function_name, playback_position_, available_frame_->description()));
                found = true;
                continue;
            } else {
                available_frame_ = std::move(frame);
                logger::log_trace(fmt::format("[{}] {} -- received {}, BREAK", function_name, playback_position_, available_frame_->description()));
                break;
            }
        } else {
            logger::log_trace(fmt::format("[{}] {} -- no new frame available", function_name, playback_position_));

            if (found) {
                logger::log_trace(fmt::format("[{}] {} -- available {}, FOUND", function_name, playback_position_, available_frame_->description()));
                should_stop = true;
                break;
            }
        }
    }

    logger::log_trace(fmt::format("[{}] {} -- received first frame, begin playback", function_name, playback_position_));

    has_received_first_real_frame_ = true;
    playback_position_ = available_frame_->timestamp();

    is_seeking_ = false;
    was_seeking_ = true;

    if (should_stop)
        stop();

    logger::log_trace(fmt::format("[{}] {} -- available {}, RETURN", function_name, playback_position_, available_frame_->description()));
    return std::move(available_frame_);
}

}  // namespace video_trimmer::video_player

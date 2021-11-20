#include "video_player.hpp"

#include "fmt/core.h"

#include "logger/logger.hpp"
#include "threaded_stream_reader/video_file.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"

VideoPlayer::VideoPlayer()
    : factory_{std::make_unique<FFmpegFactory>()}
{
}

bool VideoPlayer::open_file(const char* filename)
{
    log_debug(fmt::format("(VideoPlayer) open file: {}", filename));

    if (has_open_file())
        return false;

    video_file_ = std::make_unique<VideoFile>(filename, factory_.get());

    if (has_open_file()) {
        video_file_ = std::make_unique<VideoFile>(filename, factory_.get());
        video_content_provider_ = std::make_unique<VideoContentProvider>(factory_.get(), *video_file_, 640, 480);
        video_content_provider_->run();
    }

    return has_open_file();
}

void VideoPlayer::close_file()
{
    log_debug(fmt::format("(VideoPlayer) close file"));

    video_content_provider_->stop();
}

void VideoPlayer::start()
{
    log_debug(fmt::format("(VideoPlayer) start"));

    if (!is_playing()) {
        if (has_open_file()) {
            // begin playback
            time_point_playback_start_ = std::chrono::steady_clock::now();
            received_first_real_frame_ = false;
            is_playing_ = true;
        }
    }
}

void VideoPlayer::stop()
{
    log_debug(fmt::format("(VideoPlayer) stop"));

    if (is_playing()) {
        // stop playback
        is_playing_ = false;
    }
}

bool VideoPlayer::has_open_file()
{
    return video_file_ && video_file_->is_open();
}

bool VideoPlayer::is_playing()
{
    return has_open_file() && is_playing_;
}

bool VideoPlayer::is_stopped()
{
    return has_open_file() && !is_playing_;
}

void VideoPlayer::update()
{
    if (is_playing()) {
        // current position in playback
        if (!received_first_real_frame_) {
            time_point_playback_start_ = std::chrono::steady_clock::now();
            playback_position_ = std::chrono::steady_clock::duration::zero();
        } else {
            playback_position_ = std::chrono::steady_clock::now() - time_point_playback_start_;
        }
    }
}

std::unique_ptr<VideoFrame> VideoPlayer::next_frame()
{
    if (!is_playing())
        return nullptr;

    auto [video_frame, frames_available] = video_content_provider_->next_frame(playback_position_.count());

    if (!video_frame)
        return nullptr;

    if (!received_first_real_frame_) {
        log_debug("(VideoPlayer) received first frame, begin playback");
        received_first_real_frame_ = true;
    }

    return std::move(video_frame);
}

void VideoPlayer::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    if (has_open_file())
        video_content_provider_->change_scaling_dimensions(scale_width, scale_height);
}

double VideoPlayer::playback_position()
{
    return playback_position_.count();
}

int VideoPlayer::finished_video_frame_queue_size()
{
    return video_content_provider_ ? video_content_provider_->finished_video_frame_queue_size() : 0;
}

int VideoPlayer::video_frame_scaler_queue_size()
{
    return video_content_provider_ ? video_content_provider_->video_frame_scaler_queue_size() : 0;
}

const VideoFile* VideoPlayer::video_file()
{
    return video_file_.get();
}

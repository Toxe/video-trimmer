#include "video_player.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include "logger/logger.hpp"
#include "threaded_stream_reader/video_file.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"
#include "ui/colors.h"

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

    has_started_playing_ = false;
    is_playing_ = false;
    received_first_real_frame_ = false;
    time_point_playback_start_ = std::chrono::steady_clock::now();
    playback_position_ = std::chrono::duration<double>::zero();

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

    if (has_open_file() && !has_started_playing()) {
        has_started_playing_ = true;
        is_playing_ = true;
        received_first_real_frame_ = false;
        time_point_playback_start_ = std::chrono::steady_clock::now();
        playback_position_ = std::chrono::duration<double>::zero();
    }
}

void VideoPlayer::resume()
{
    log_debug(fmt::format("(VideoPlayer) resume"));

    if (has_started_playing() && is_paused()) {
        is_playing_ = true;
    }
}

void VideoPlayer::pause()
{
    log_debug(fmt::format("(VideoPlayer) pause"));

    if (has_started_playing() && is_playing()) {
        is_playing_ = false;
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

void VideoPlayer::change_scaling_dimensions(ImageSize image_size)
{
    if (has_open_file())
        video_content_provider_->change_scaling_dimensions(image_size.width, image_size.height);
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

void VideoPlayer::render()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("playback controls");

    ImGui::TextColored(has_open_file() ? UserInterface::Colors::green : UserInterface::Colors::red, fmt::format("has_open_file").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(has_started_playing() ? UserInterface::Colors::green : UserInterface::Colors::red, fmt::format("has_started_playing").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(is_playing() ? UserInterface::Colors::green : UserInterface::Colors::red, fmt::format("is_playing").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(is_paused() ? UserInterface::Colors::green : UserInterface::Colors::red, fmt::format("is_paused").c_str());
    ImGui::SameLine();

    if (has_open_file()) {
        if (!has_started_playing()) {
            if (ImGui::Button("start")) {
                start();
            }
        }

        if (is_playing()) {
            if (ImGui::Button("pause")) {
                pause();
            }
        }

        if (is_paused()) {
            if (ImGui::Button("resume")) {
                resume();
            }
        }

        ImGui::SameLine();
    }

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}
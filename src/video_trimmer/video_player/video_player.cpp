#include "video_player.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "video_trimmer/logger/logger.hpp"
#include "video_trimmer/ui/colors/colors.hpp"

namespace video_trimmer::video_player {

bool VideoPlayer::open_file(const char* filename)
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

    return has_open_file();
}

void VideoPlayer::close_file()
{
    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) close file"));
}

void VideoPlayer::start()
{
    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) start"));

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
    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) resume"));

    if (has_started_playing() && is_paused()) {
        is_playing_ = true;
    }
}

void VideoPlayer::pause()
{
    video_trimmer::logger::log_debug(fmt::format("(VideoPlayer) pause"));

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

void VideoPlayer::render()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("playback controls");

    ImGui::TextColored(has_open_file() ? video_trimmer::ui::colors::green : video_trimmer::ui::colors::red, "%s", fmt::format("has_open_file").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(has_started_playing() ? video_trimmer::ui::colors::green : video_trimmer::ui::colors::red, "%s", fmt::format("has_started_playing").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(is_playing() ? video_trimmer::ui::colors::green : video_trimmer::ui::colors::red, "%s", fmt::format("is_playing").c_str());
    ImGui::SameLine();
    ImGui::Text("|");
    ImGui::SameLine();
    ImGui::TextColored(is_paused() ? video_trimmer::ui::colors::green : video_trimmer::ui::colors::red, "%s", fmt::format("is_paused").c_str());
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

}  // namespace video_trimmer::video_player

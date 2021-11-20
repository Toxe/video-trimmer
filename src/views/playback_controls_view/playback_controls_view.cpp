#include "playback_controls_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

void PlaybackControlsView::render(const double playback_position, const int finished_video_frame_queue_size, const int video_frame_scaler_queue_size)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("playback controls");

    ImGui::Text(fmt::format("playback controls [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::Text(fmt::format("playback position: {:.4f}", playback_position).c_str());
    ImGui::Text(fmt::format("video frame scaler queue: {}, finished video frames queue: {}", video_frame_scaler_queue_size, finished_video_frame_queue_size).c_str());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

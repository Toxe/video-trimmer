#include "playback_controls_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

void PlaybackControlsView::render(double playback_position)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("playback controls");

    ImGui::Text(fmt::format("playback controls [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::Text(fmt::format("playback position: {:.4f}", playback_position).c_str());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

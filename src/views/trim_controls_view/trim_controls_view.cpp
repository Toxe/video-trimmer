#include "trim_controls_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

void TrimControlsView::render()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("trim controls");

    ImGui::Text(fmt::format("trim controls [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

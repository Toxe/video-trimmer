#include "files_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

void FilesView::render()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("left pane");
    ImGui::BeginChild("files");

    ImGui::Text(fmt::format("files [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

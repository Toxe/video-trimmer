#include "additional_info_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include "command_line/command_line.h"

AdditionalInfoView::AdditionalInfoView(const video_trimmer::CommandLine& cli)
    : font_size_{static_cast<float>(cli.font_size())}, fps_widget_{font_size_}, memory_usage_widget_{font_size_}
{
}

void AdditionalInfoView::render(const Duration elapsed_time, const VideoFile* video_file)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("left pane");
    ImGui::BeginChild("additional info");

    fps_widget_.render(elapsed_time);
    memory_usage_widget_.render(elapsed_time);

    ImGui::Text(fmt::format("additional info [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    video_file_info_widget_.render(video_file);

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

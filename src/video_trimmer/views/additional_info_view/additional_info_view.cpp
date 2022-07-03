#include "additional_info_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

namespace video_trimmer::views::additional_info_view {

AdditionalInfoView::AdditionalInfoView(const video_trimmer::command_line::CommandLine& cli)
    : font_size_{static_cast<float>(cli.font_size())}, fps_widget_{font_size_}, memory_usage_widget_{font_size_}
{
}

void AdditionalInfoView::render(const video_trimmer::clock::Duration elapsed_time, const VideoFile* video_file)
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

}  // namespace video_trimmer::views::additional_info_view

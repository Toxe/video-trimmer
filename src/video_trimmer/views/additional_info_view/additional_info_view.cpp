#include "additional_info_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

namespace video_trimmer::views::additional_info_view {

void AdditionalInfoView::render(const clock::Duration previous_frame_time, const video_file::VideoFile* video_file)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("left pane");
    ImGui::BeginChild("additional info");

    fps_widget_.render();
    frame_time_widget_.render(previous_frame_time);
    memory_usage_widget_.render();

    ImGui::TextUnformatted(fmt::format("additional info [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    video_file_info_widget_.render(video_file);

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

}  // namespace video_trimmer::views::additional_info_view

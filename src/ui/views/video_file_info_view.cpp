#include "video_file_info_view.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include "video/video_file.hpp"
#include "ui/colors.h"

void VideoFileInfoView::render(const VideoFile& video_file)
{
    if (!video_file.is_open())
        return;

    ImGui::NewLine();
    ImGui::TextColored(UserInterface::Colors::light_blue, video_file.filename().c_str());

    const int num_streams = video_file.number_of_streams();

    if (num_streams > 0) {
        for (int stream_id = 0; stream_id < num_streams; ++stream_id) {
            const auto stream_info = video_file.stream_info(stream_id);
            ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("stream #{} ({}):", stream_id, stream_info.type).c_str());
            ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", stream_info.codec).c_str());
            ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", stream_info.additional_info).c_str());
        }
    }
}

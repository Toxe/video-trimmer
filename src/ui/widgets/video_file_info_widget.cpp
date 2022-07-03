#include "video_file_info_widget.hpp"

#include <fmt/core.h>
#include <imgui.h>

#include "ui/colors.h"

void VideoFileInfoWidget::render(const VideoFile* video_file)
{
    if (!video_file || !video_file->is_open())
        return;

    ImGui::NewLine();
    ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("{} ({})", video_file->filename(), video_file->file_format()).c_str());

    if (video_file->has_video_stream()) {
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("stream #{} ({}):", video_file->video_stream_info()->stream_index(), video_file->video_stream_info()->codec_type()).c_str());
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", video_file->video_stream_info()->codec_name()).c_str());
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", video_file->video_stream_info()->codec_additional_info()).c_str());
    }

    if (video_file->has_audio_stream()) {
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("stream #{} ({}):", video_file->audio_stream_info()->stream_index(), video_file->audio_stream_info()->codec_type()).c_str());
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", video_file->audio_stream_info()->codec_name()).c_str());
        ImGui::TextColored(UserInterface::Colors::light_blue, fmt::format("    {}", video_file->audio_stream_info()->codec_additional_info()).c_str());
    }
}

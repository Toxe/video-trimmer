#include "video_file_info_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "../../colors/colors.hpp"

namespace video_trimmer::ui::widgets::video_file_info_widget {

void VideoFileInfoWidget::render(const video_reader::video_file::VideoFile* video_file)
{
    if (!video_file || !video_file->is_open())
        return;

    ImGui::NewLine();
    ImGui::TextColored(colors::light_blue, "%s", fmt::format("{} ({})", video_file->filename(), video_file->file_format()).c_str());

    if (video_file->has_video_stream()) {
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("stream #{} ({}):", video_file->video_stream_info()->stream_index(), video_file->video_stream_info()->codec_type()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", video_file->video_stream_info()->codec_name()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", video_file->video_stream_info()->codec_additional_info()).c_str());
    }

    if (video_file->has_audio_stream()) {
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("stream #{} ({}):", video_file->audio_stream_info()->stream_index(), video_file->audio_stream_info()->codec_type()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", video_file->audio_stream_info()->codec_name()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", video_file->audio_stream_info()->codec_additional_info()).c_str());
    }
}

}  // namespace video_trimmer::ui::widgets::video_file_info_widget

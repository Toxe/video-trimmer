#include "video_file_info_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "../../colors/colors.hpp"
#include "video_trimmer/video_file/video_file.hpp"

namespace video_trimmer::ui::widgets::video_file_info_widget {

void VideoFileInfoWidget::render(const video_file::VideoFile* video_file)
{
    if (!video_file || !video_file->is_open())
        return;

    ImGui::NewLine();
    ImGui::TextColored(colors::light_blue, "%s", video_file->filename().c_str());
    ImGui::TextColored(colors::light_blue, "%s", video_file->file_format().c_str());

    ImGui::NewLine();

    if (video_file->has_video_stream()) {
        const video_file::CodecContext* codec_content = video_file->video_codec_context();

        ImGui::TextColored(colors::light_blue, "%s", fmt::format("stream #{} ({}):", codec_content->stream_index(), codec_content->codec_type()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", codec_content->codec_name()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", codec_content->codec_additional_info()).c_str());
    }

    if (video_file->has_audio_stream()) {
        const video_file::CodecContext* codec_context = video_file->audio_codec_context();

        ImGui::TextColored(colors::light_blue, "%s", fmt::format("stream #{} ({}):", codec_context->stream_index(), codec_context->codec_type()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", codec_context->codec_name()).c_str());
        ImGui::TextColored(colors::light_blue, "%s", fmt::format("    {}", codec_context->codec_additional_info()).c_str());
    }
}

}  // namespace video_trimmer::ui::widgets::video_file_info_widget

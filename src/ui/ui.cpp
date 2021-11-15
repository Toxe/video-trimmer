#include "ui.h"

#include <fmt/core.h>

#include "clock/duration.h"
#include "colors.h"
#include "command_line/command_line.h"
#include "event_handler/event_handler.h"

const float left_pane_width = 500.0f;
const float additional_info_pane_height = 400.0f;
const float playback_controls_pane_height = 100.0f;
const float trim_controls_pane_height = 150.0f;

UI::UI(const CommandLine& cli)
    : font_size_{static_cast<float>(cli.font_size())}, fps_view_{font_size_}
{
}

void UI::render(const Duration elapsed_time, const VideoFile& video_file)
{
    render_main_window(elapsed_time, video_file);
    render_help_window();
}

void UI::render_main_window(const Duration elapsed_time, const VideoFile& video_file)
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(main_window_title_, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    render_left_pane(left_pane_width, elapsed_time, video_file);

    ImGui::SameLine();

    render_right_pane();

    ImGui::End();
}

void UI::render_help_window()
{
    if (show_help_) {
        ImGui::SetNextWindowPos(ImVec2(20 + 20 + main_window_size_.x, 20), ImGuiCond_FirstUseEver);
        ImGui::Begin(help_window_title_, &show_help_, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::TextColored(UserInterface::Colors::light_blue, "   F1");
        ImGui::SameLine();
        ImGui::Text("show/hide help");

        ImGui::TextColored(UserInterface::Colors::light_blue, "  ESC");
        ImGui::SameLine();
        ImGui::Text("quit");

        if (ImGui::Button("Close"))
            event_handler_->handle_event(Event::ToggleHelp);

        ImGui::End();
    }
}

void UI::help(const std::string& text)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void UI::render_left_pane(const float pane_width, const Duration elapsed_time, const VideoFile& video_file)
{
    ImGui::BeginChild("left pane", ImVec2(pane_width, 0), false, ImGuiWindowFlags_None);

    const float files_pane_height = ImGui::GetWindowSize().y - additional_info_pane_height - ImGui::GetStyle().ItemSpacing.y;

    render_files_pane(files_pane_height);
    render_additional_info_pane(additional_info_pane_height, elapsed_time, video_file);

    ImGui::EndChild();
}

void UI::render_right_pane()
{
    ImGui::BeginChild("right pane", ImVec2(0, 0), false, ImGuiWindowFlags_None);

    const float video_pane_height = ImGui::GetWindowSize().y - playback_controls_pane_height - trim_controls_pane_height - 2.0f * ImGui::GetStyle().ItemSpacing.y;

    render_video_pane(video_pane_height);
    render_playback_controls_pane(playback_controls_pane_height);
    render_trim_controls_pane(trim_controls_pane_height);

    ImGui::EndChild();
}

void UI::render_files_pane(const float pane_height)
{
    ImGui::BeginChild("files pane", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::Text("Files");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::EndChild();
}

void UI::render_additional_info_pane(const float pane_height, const Duration elapsed_time, const VideoFile& video_file)
{
    ImGui::BeginChild("additional info pane", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);

    fps_view_.render(elapsed_time);

    ImGui::Text("Additional Info");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());

    video_file_info_view_.render(video_file);

    ImGui::EndChild();
}

void UI::render_video_pane(const float pane_height)
{
    ImGui::BeginChild("video pane", ImVec2(0, pane_height), false, ImGuiWindowFlags_None);

    video_view_position_ = ImGui::GetCursorScreenPos();
    video_view_size_ = ImGui::GetWindowSize();

    ImGui::Text("Video");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::EndChild();
}

void UI::render_playback_controls_pane(const float pane_height)
{
    ImGui::BeginChild("playback controls pane", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::Text("Playback Controls");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::EndChild();
}

void UI::render_trim_controls_pane(const float pane_height)
{
    ImGui::BeginChild("trim controls pane", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::Text("Trim Controls");
    ImGui::Text(fmt::format("{}x{}", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::EndChild();
}

ImagePosition UI::video_view_position() const
{
    return {static_cast<int>(video_view_position_.x), static_cast<int>(video_view_position_.y)};
}

ImageSize UI::video_view_size() const
{
    return {static_cast<int>(video_view_size_.x), static_cast<int>(video_view_size_.y)};
}

#include "ui.hpp"

#include <fmt/core.h>

#include "colors/colors.hpp"
#include "event_handler/events.hpp"

namespace video_trimmer::ui {

const float left_pane_width = 500.0f;
const float additional_info_pane_height = 500.0f;
const float playback_controls_pane_height = 100.0f;
const float trim_controls_pane_height = 150.0f;

void UI::render()
{
    render_main_window();
    render_help_window();

    // ImGui::ShowMetricsWindow();
}

void UI::render_main_window()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(main_window_title_, nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

    setup_left_pane(left_pane_width);

    ImGui::SameLine();

    setup_right_pane();

    ImGui::End();
}

void UI::render_help_window()
{
    if (show_help_) {
        ImGui::SetNextWindowPos(ImVec2(20 + 20 + main_window_size_.x, 20), ImGuiCond_FirstUseEver);
        ImGui::Begin(help_window_title_, &show_help_, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

        ImGui::TextColored(video_trimmer::ui::colors::light_blue, "   F1");
        ImGui::SameLine();
        ImGui::Text("show/hide help");

        ImGui::TextColored(video_trimmer::ui::colors::light_blue, "  ESC");
        ImGui::SameLine();
        ImGui::Text("quit");

        if (ImGui::Button("Close"))
            event_handler_->handle_event(video_trimmer::event_handler::Event::ToggleHelp);

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

void UI::setup_left_pane(const float pane_width)
{
    ImGui::BeginChild("left pane", ImVec2(pane_width, 0), false, ImGuiWindowFlags_None);

    const float files_pane_height = ImGui::GetWindowSize().y - additional_info_pane_height - ImGui::GetStyle().ItemSpacing.y;

    setup_files_view(files_pane_height);
    setup_additional_info_view(additional_info_pane_height);

    ImGui::EndChild();
}

void UI::setup_right_pane()
{
    ImGui::BeginChild("right pane", ImVec2(0, 0), false, ImGuiWindowFlags_None);

    const float video_pane_height = ImGui::GetWindowSize().y - playback_controls_pane_height - trim_controls_pane_height - 2.0f * ImGui::GetStyle().ItemSpacing.y;

    setup_video_view(video_pane_height);
    setup_playback_controls_pane(playback_controls_pane_height);
    setup_trim_controls_pane(trim_controls_pane_height);

    ImGui::EndChild();
}

void UI::setup_files_view(const float pane_height)
{
    ImGui::BeginChild("files", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::EndChild();
}

void UI::setup_additional_info_view(const float pane_height)
{
    ImGui::BeginChild("additional info", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::EndChild();
}

void UI::setup_video_view(const float pane_height)
{
    ImGui::BeginChild("video", ImVec2(0, pane_height), false, ImGuiWindowFlags_None);
    ImGui::EndChild();
}

void UI::setup_playback_controls_pane(const float pane_height)
{
    ImGui::BeginChild("playback controls", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::EndChild();
}

void UI::setup_trim_controls_pane(const float pane_height)
{
    ImGui::BeginChild("trim controls", ImVec2(0, pane_height), true, ImGuiWindowFlags_None);
    ImGui::EndChild();
}

}  // namespace video_trimmer::ui

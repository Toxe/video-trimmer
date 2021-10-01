#include "ui.h"

#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <limits>
#include <numbers>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include "clock/duration.h"
#include "colors.h"
#include "command_line/command_line.h"

UI::UI(const CommandLine& cli)
    : font_size_{static_cast<float>(cli.font_size())}
{
}

void UI::render(const Duration elapsed_time)
{
    render_main_window(elapsed_time);
    render_help_window();
}

void UI::render_main_window(const Duration elapsed_time)
{
    static std::vector<float> fps(120);
    static std::size_t values_offset = 0;

    const float elapsed_time_in_seconds = elapsed_time.as_seconds();
    const float current_fps = elapsed_time.fps();
    const auto fps_label = fmt::format("{:.1f} FPS ({:.3f} ms/frame)", current_fps, 1000.0f * elapsed_time_in_seconds);
    fps[values_offset] = current_fps;
    values_offset = (values_offset + 1) % fps.size();

    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);
    ImGui::Begin(main_window_title_, nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::PlotLines("", fps.data(), static_cast<int>(fps.size()), static_cast<int>(values_offset), fps_label.c_str(), 0.0f, 1.5f * std::max(65.0f, *std::max_element(fps.begin(), fps.end())), ImVec2(0, 4.0f * font_size_));

    if (ImGui::Button("Help (F1)"))
        event_handler_->handle_event(Event::ToggleHelp);

    main_window_size_ = ImGui::GetWindowSize();
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

bool UI::input_int(const char* label, InputValue<int>& value, const int small_inc, const int big_inc, const int min, const int max)
{
    bool changed_now = false;
    int val = value.get();

    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);

    if (ImGui::InputInt(label, &val, small_inc, big_inc)) {
        value.set(std::clamp(val, min, max));
        changed_now = true;
    }

    ImGui::SameLine();
    help(fmt::format("{} to {}\n\n     -/+ to change by {}\nCTRL -/+ to change by {}", min, max, small_inc, big_inc));

    return changed_now;
}

bool UI::input_double(const char* label, InputValue<double>& value, const double small_inc, const double big_inc, const double min, const double max)
{
    bool changed_now = false;
    double val = value.get();

    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 15);

    if (ImGui::InputDouble(label, &val, small_inc, big_inc, "%.16lf")) {
        value.set(std::clamp(val, min, max));
        changed_now = true;
    }

    ImGui::SameLine();
    help(fmt::format("{} to {}\n\n     -/+ to change by {}\nCTRL -/+ to change by {}", min, max, small_inc, big_inc));

    return changed_now;
}

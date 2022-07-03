#include "fps_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

namespace video_trimmer::ui::widgets::fps_widget {

FPSWidget::FPSWidget(const float font_size)
    : font_size_{font_size}
{
    fps_.resize(5 * 60, 0.0f);  // 5 seconds worth of values at 60 FPS
}

void FPSWidget::render(const video_trimmer::clock::Duration elapsed_time)
{
    const float elapsed_time_in_seconds = elapsed_time.as_seconds();
    const float current_fps = elapsed_time.fps();
    const auto fps_label = fmt::format("{:.1f} FPS ({:.3f} ms/frame)", current_fps, 1000.0f * elapsed_time_in_seconds);

    fps_[values_offset_] = current_fps;
    values_offset_ = (values_offset_ + 1) % fps_.size();

    ImGui::PlotLines("", fps_.data(), static_cast<int>(fps_.size()), static_cast<int>(values_offset_), fps_label.c_str(), 0.0f, 1.5f * std::max(65.0f, *std::max_element(fps_.begin(), fps_.end())), ImVec2(ImGui::GetContentRegionAvail().x, 4.0f * font_size_));
}

}  // namespace video_trimmer::ui::widgets::fps_widget

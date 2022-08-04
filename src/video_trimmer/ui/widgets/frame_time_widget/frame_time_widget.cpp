#include "frame_time_widget.hpp"

#include <algorithm>

#include "fmt/core.h"
#include "imgui.h"

namespace video_trimmer::ui::widgets::frame_time_widget {

constexpr float graph_height = 70.0f;

FrameTimeWidget::FrameTimeWidget()
{
    values_.resize(5 * 60, 0.0f);  // 5 seconds worth of values at 60 FPS
}

void FrameTimeWidget::render(const clock::Duration frame_time)
{
    const float ms = frame_time.as_seconds() * 1000.0f;

    values_[values_offset_] = ms;
    values_offset_ = (values_offset_ + 1) % values_.size();

    const auto fps_label = fmt::format("Frame time: {:6.3f} ms", ms);
    ImGui::PlotLines("", values_.data(), static_cast<int>(values_.size()), static_cast<int>(values_offset_), fps_label.c_str(), 0.0f, 1.5f * *std::max_element(values_.begin(), values_.end()), ImVec2(ImGui::GetContentRegionAvail().x, graph_height));
}

}  // namespace video_trimmer::ui::widgets::frame_time_widget

#include "frame_time_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

namespace video_trimmer::ui::widgets::frame_time_widget {

constexpr float graph_height = 70.0f;

FrameTimeWidget::FrameTimeWidget()
    : values_(5 * 60)  // 5 seconds worth of values at 60 FPS
{
}

void FrameTimeWidget::render(const clock::Duration frame_time)
{
    values_.push(frame_time.as_seconds() * 1000.0f);

    const auto fps_label = fmt::format("Frame time: {:6.3f} ms", values_.last());
    ImGui::PlotLines("", values_.data(), values_.size(), values_.offset(), fps_label.c_str(), 0.0f, 1.5f * values_.max(), ImVec2(ImGui::GetContentRegionAvail().x, graph_height));
}

}  // namespace video_trimmer::ui::widgets::frame_time_widget

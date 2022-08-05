#include "fps_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

namespace video_trimmer::ui::widgets::fps_widget {

constexpr float graph_height = 70.0f;
constexpr float time_between_avg_fps_updates = 1.0f / 5.0f;

FPSWidget::FPSWidget()
    : fps_values_(5 * 60)  // 5 seconds worth of values at 60 FPS
{
    last_avg_fps_update_time_ = std::chrono::steady_clock::now();
}

void FPSWidget::render()
{
    const float current_fps = 1.0f / ImGui::GetIO().DeltaTime;

    fps_values_.push(current_fps);

    avg_fps_accum_ += current_fps;
    ++avg_fps_count_;

    const std::chrono::duration<float> time_since_last_update = std::chrono::steady_clock::now() - last_avg_fps_update_time_;

    if (time_since_last_update.count() >= time_between_avg_fps_updates) {
        avg_fps_ = (avg_fps_count_ > 0) ? avg_fps_accum_ / static_cast<float>(avg_fps_count_) : 0.0f;
        avg_fps_accum_ = 0.0f;
        avg_fps_count_ = 0;

        last_avg_fps_update_time_ = std::chrono::steady_clock::now();
    }

    const auto fps_label = fmt::format("{:6.1f} FPS ({:.1f} avg.)", current_fps, avg_fps_);
    ImGui::PlotLines("", fps_values_.data(), fps_values_.size(), fps_values_.offset(), fps_label.c_str(), 0.0f, 1.5f * fps_values_.max(), ImVec2(ImGui::GetContentRegionAvail().x, graph_height));
}

}  // namespace video_trimmer::ui::widgets::fps_widget

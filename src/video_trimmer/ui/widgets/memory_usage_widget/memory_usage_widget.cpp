#include "memory_usage_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "memory_usage_info/create_memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

constexpr float graph_height = 70.0f;
constexpr float time_between_updates = 1.0f / 15.0f;

MemoryUsageWidget::MemoryUsageWidget()
    : values_(10 * 30)  // 10 seconds worth of values at 30 FPS
{
    memory_usage_ = memory_usage_info::create_memory_usage_info();
    last_update_time_ = std::chrono::steady_clock::now();
}

void MemoryUsageWidget::render()
{
    const std::chrono::duration<float> time_since_last_update = std::chrono::steady_clock::now() - last_update_time_;

    if (time_since_last_update.count() >= time_between_updates) {
        values_.push(memory_usage_->get_memory_usage());
        last_update_time_ = std::chrono::steady_clock::now();
    }

    const auto label = fmt::format("Memory Usage: {:.3f} MB", values_.last());
    ImGui::PlotLines("", values_.data(), values_.size(), values_.offset(), label.c_str(), 0.0f, 1.5f * values_.max(), ImVec2(ImGui::GetContentRegionAvail().x, graph_height));
}

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

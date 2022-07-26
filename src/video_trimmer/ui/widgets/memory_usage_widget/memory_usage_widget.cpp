#include "memory_usage_widget.hpp"

#include <algorithm>

#include "fmt/core.h"
#include "imgui.h"

#include "memory_usage_info/create_memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

constexpr float graph_height = 70.0f;
constexpr float time_between_updates = 1.0f / 30.0f;

MemoryUsageWidget::MemoryUsageWidget()
{
    memory_usage_ = memory_usage_info::create_memory_usage_info();

    mem_.resize(10 * 30, 0.0f);  // 10 seconds worth of values at 30 FPS

    last_update_time_ = std::chrono::steady_clock::now();
}

void MemoryUsageWidget::render()
{
    const std::chrono::duration<float> time_since_last_update = std::chrono::steady_clock::now() - last_update_time_;

    if (time_since_last_update.count() >= time_between_updates) {
        current_memory_usage_ = memory_usage_->get_memory_usage();
        mem_[values_offset_] = current_memory_usage_;
        values_offset_ = (values_offset_ + 1) % mem_.size();
        last_update_time_ = std::chrono::steady_clock::now();
    }

    const auto label = fmt::format("Memory Usage: {:.3f} MB", current_memory_usage_);
    ImGui::PlotLines("", mem_.data(), static_cast<int>(mem_.size()), static_cast<int>(values_offset_), label.c_str(), 0.0f, 1.5f * std::max(65.0f, *std::max_element(mem_.begin(), mem_.end())), ImVec2(ImGui::GetContentRegionAvail().x, graph_height));
}

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

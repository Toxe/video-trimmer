#include "memory_usage_widget.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "memory_usage_info/create_memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget {

MemoryUsageWidget::MemoryUsageWidget(const float font_size)
    : font_size_{font_size}
{
    memory_usage_ = create_memory_usage_info();

    mem_.resize(5 * 60, 0.0f);  // 5 seconds worth of values at 60 FPS
}

void MemoryUsageWidget::render(const video_trimmer::clock::Duration elapsed_time)
{
    const float current_memory_usage = memory_usage_->get_memory_usage();
    const auto label = fmt::format("Memory Usage: {:.3f} MB", current_memory_usage);

    mem_[values_offset_] = current_memory_usage;
    values_offset_ = (values_offset_ + 1) % mem_.size();

    ImGui::PlotLines("", mem_.data(), static_cast<int>(mem_.size()), static_cast<int>(values_offset_), label.c_str(), 0.0f, 1.5f * std::max(65.0f, *std::max_element(mem_.begin(), mem_.end())), ImVec2(ImGui::GetContentRegionAvail().x, 4.0f * font_size_));
}

}  // namespace video_trimmer::ui::widgets::memory_usage_widget

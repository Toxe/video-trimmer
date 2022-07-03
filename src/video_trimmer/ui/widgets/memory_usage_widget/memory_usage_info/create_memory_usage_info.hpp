#pragma once

#include <memory>

#include "memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info {

[[nodiscard]] std::unique_ptr<MemoryUsageInfo> create_memory_usage_info();

}  // namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info

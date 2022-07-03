#pragma once

#include "memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info {

class LinuxMemoryUsageInfo : public MemoryUsageInfo {
public:
    [[nodiscard]] float get_memory_usage() override;
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info

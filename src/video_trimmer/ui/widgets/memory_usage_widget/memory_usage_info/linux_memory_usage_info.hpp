#pragma once

#include "memory_usage_info.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info {

class LinuxMemoryUsageInfo : public MemoryUsageInfo {
public:
    LinuxMemoryUsageInfo();

    [[nodiscard]] float get_memory_usage() override;

private:
    int page_size_;
};

}  // namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info

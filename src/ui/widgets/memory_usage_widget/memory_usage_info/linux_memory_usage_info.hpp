#pragma once

#include "memory_usage_info.hpp"

class LinuxMemoryUsageInfo : public MemoryUsageInfo {
public:
    [[nodiscard]] float get_memory_usage() override;
};

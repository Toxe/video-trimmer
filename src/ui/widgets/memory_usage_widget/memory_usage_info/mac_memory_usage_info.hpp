#pragma once

#include "memory_usage_info.hpp"

class MacMemoryUsageInfo : public MemoryUsageInfo {
public:
    [[nodiscard]] float get_memory_usage() override;
};

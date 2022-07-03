#pragma once

#include <memory>

class MemoryUsageInfo;

[[nodiscard]] std::unique_ptr<MemoryUsageInfo> create_memory_usage_info();

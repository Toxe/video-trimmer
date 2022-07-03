#pragma once

#include <memory>

#include "memory_usage_info.hpp"

[[nodiscard]] std::unique_ptr<MemoryUsageInfo> create_memory_usage_info();

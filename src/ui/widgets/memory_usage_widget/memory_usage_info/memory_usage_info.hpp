#pragma once

constexpr float mbyte = 1024.0f * 1024.0f;

class MemoryUsageInfo {
public:
    virtual ~MemoryUsageInfo() = default;

    [[nodiscard]] virtual float get_memory_usage() = 0;

protected:
    [[nodiscard]] static float to_mbyte(const float bytes) { return bytes / mbyte; };
};

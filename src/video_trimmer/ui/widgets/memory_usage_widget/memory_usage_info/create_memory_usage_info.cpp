#include "create_memory_usage_info.hpp"

#if __linux__
#include "linux_memory_usage_info.hpp"
#elif __MACH__
#include "mac_memory_usage_info.hpp"
#elif _WIN32 || _WIN64
#include "windows_memory_usage_info.hpp"
#endif

std::unique_ptr<MemoryUsageInfo> create_memory_usage_info()
{
#if __linux__
    return std::make_unique<LinuxMemoryUsageInfo>();
#elif __MACH__
    return std::make_unique<MacMemoryUsageInfo>();
#elif _WIN32 || _WIN64
    return std::make_unique<WindowsMemoryUsageInfo>();
#endif
}

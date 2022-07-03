#include "windows_memory_usage_info.hpp"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <psapi.h>

namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info {

float WindowsMemoryUsageInfo::get_memory_usage()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));

    return to_mbyte(static_cast<float>(pmc.WorkingSetSize));
}

}  // namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info

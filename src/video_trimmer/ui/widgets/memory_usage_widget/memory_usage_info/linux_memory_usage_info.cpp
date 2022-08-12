#include "linux_memory_usage_info.hpp"

#include <fstream>

#include <unistd.h>

#include "../../../../error/error.hpp"

namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info {

LinuxMemoryUsageInfo::LinuxMemoryUsageInfo()
{
    page_size_ = static_cast<int>(sysconf(_SC_PAGESIZE));
}

float LinuxMemoryUsageInfo::get_memory_usage()
{
    std::ifstream input{"/proc/self/statm"};

    if (!input.is_open())
        error::die("unable to open file: /proc/self/statm");

    int size, resident;
    input >> size >> resident;

    return to_mbyte(static_cast<float>(resident * page_size_));
}

}  // namespace video_trimmer::ui::widgets::memory_usage_widget::memory_usage_info

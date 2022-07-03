#pragma once

#include "clock/duration.h"
#include "command_line/command_line.h"
#include "ui/widgets/fps_widget.hpp"
#include "ui/widgets/memory_usage_widget/memory_usage_widget.hpp"
#include "ui/widgets/video_file_info_widget.hpp"

class AdditionalInfoView {
    float font_size_;

    FPSWidget fps_widget_;
    MemoryUsageWidget memory_usage_widget_;
    VideoFileInfoWidget video_file_info_widget_;

public:
    AdditionalInfoView(const video_trimmer::CommandLine& cli);

    void render(Duration elapsed_time, const VideoFile* video_file);
};

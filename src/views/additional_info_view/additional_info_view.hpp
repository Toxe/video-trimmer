#pragma once

#include "clock/duration.hpp"
#include "command_line/command_line.h"
#include "ui/widgets/fps_widget/fps_widget.hpp"
#include "ui/widgets/memory_usage_widget/memory_usage_widget.hpp"
#include "ui/widgets/video_file_info_widget/video_file_info_widget.hpp"

class AdditionalInfoView {
    float font_size_;

    video_trimmer::ui::widgets::fps_widget::FPSWidget fps_widget_;
    video_trimmer::ui::widgets::memory_usage_widget::MemoryUsageWidget memory_usage_widget_;
    video_trimmer::ui::widgets::video_file_info_widget::VideoFileInfoWidget video_file_info_widget_;

public:
    AdditionalInfoView(const video_trimmer::CommandLine& cli);

    void render(video_trimmer::clock::Duration elapsed_time, const VideoFile* video_file);
};

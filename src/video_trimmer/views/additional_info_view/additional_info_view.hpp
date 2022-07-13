#pragma once

#include "video_trimmer/clock/duration.hpp"
#include "video_trimmer/command_line/command_line.h"
#include "video_trimmer/ui/widgets/fps_widget/fps_widget.hpp"
#include "video_trimmer/ui/widgets/memory_usage_widget/memory_usage_widget.hpp"
#include "video_trimmer/ui/widgets/video_file_info_widget/video_file_info_widget.hpp"

namespace video_trimmer::views::additional_info_view {

class AdditionalInfoView {
public:
    AdditionalInfoView(const video_trimmer::command_line::CommandLine& cli);

    void render(video_trimmer::clock::Duration elapsed_time, const video_content_provider::video_file::VideoFile* video_file);

private:
    video_trimmer::ui::widgets::fps_widget::FPSWidget fps_widget_;
    video_trimmer::ui::widgets::memory_usage_widget::MemoryUsageWidget memory_usage_widget_;
    video_trimmer::ui::widgets::video_file_info_widget::VideoFileInfoWidget video_file_info_widget_;
};

}  // namespace video_trimmer::views::additional_info_view

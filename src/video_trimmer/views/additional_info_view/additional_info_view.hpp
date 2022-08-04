#pragma once

#include "video_trimmer/clock/duration.hpp"
#include "video_trimmer/ui/widgets/fps_widget/fps_widget.hpp"
#include "video_trimmer/ui/widgets/frame_time_widget/frame_time_widget.hpp"
#include "video_trimmer/ui/widgets/memory_usage_widget/memory_usage_widget.hpp"
#include "video_trimmer/ui/widgets/video_file_info_widget/video_file_info_widget.hpp"

namespace video_trimmer::views::additional_info_view {

class AdditionalInfoView {
public:
    void render(clock::Duration previous_frame_time, const video_file::VideoFile* video_file);

private:
    ui::widgets::fps_widget::FPSWidget fps_widget_;
    ui::widgets::frame_time_widget::FrameTimeWidget frame_time_widget_;
    ui::widgets::memory_usage_widget::MemoryUsageWidget memory_usage_widget_;
    ui::widgets::video_file_info_widget::VideoFileInfoWidget video_file_info_widget_;
};

}  // namespace video_trimmer::views::additional_info_view

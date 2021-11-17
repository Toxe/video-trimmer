#pragma once

#include "clock/duration.h"
#include "ui/widgets/fps_widget.hpp"
#include "ui/widgets/video_file_info_widget.hpp"

class CommandLine;
class VideoFile;

class AdditionalInfoView {
    float font_size_;

    FPSWidget fps_widget_;
    VideoFileInfoWidget video_file_info_widget_;

public:
    AdditionalInfoView(const CommandLine& cli);

    void render(Duration elapsed_time, const VideoFile& video_file);
};

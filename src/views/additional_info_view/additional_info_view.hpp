#pragma once

#include "clock/duration.h"
#include "ui/views/fps_view.h"
#include "ui/views/video_file_info_view.hpp"

class CommandLine;
class VideoFile;

class AdditionalInfoView {
    float font_size_;

    FPSView fps_view_;
    VideoFileInfoView video_file_info_view_;

public:
    AdditionalInfoView(const CommandLine& cli);

    void render(Duration elapsed_time, const VideoFile& video_file);
};

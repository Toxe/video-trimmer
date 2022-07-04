#pragma once

#include "video_trimmer/video_content_provider/video_file.hpp"

namespace video_trimmer::ui::widgets::video_file_info_widget {

class VideoFileInfoWidget {
public:
    void render(const VideoFile* video_file);
};

}  // namespace video_trimmer::ui::widgets::video_file_info_widget

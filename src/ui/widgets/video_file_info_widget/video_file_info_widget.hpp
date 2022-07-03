#pragma once

#include "threaded_stream_reader/video_file.hpp"

namespace video_trimmer::ui::widgets::video_file_info_widget {

class VideoFileInfoWidget {
public:
    void render(const VideoFile* video_file);
};

}  // namespace video_trimmer::ui::widgets::video_file_info_widget

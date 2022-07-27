#pragma once

namespace video_trimmer::video_file {
class VideoFile;
}  // namespace video_trimmer::video_file

namespace video_trimmer::ui::widgets::video_file_info_widget {

class VideoFileInfoWidget {
public:
    void render(const video_file::VideoFile* video_file);
};

}  // namespace video_trimmer::ui::widgets::video_file_info_widget

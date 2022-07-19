#pragma once

#include <optional>
#include <string>

namespace video_trimmer::command_line {

class CommandLine {
public:
    CommandLine(int argc, char* argv[]);

    [[nodiscard]] int font_size() const { return font_size_; }

    [[nodiscard]] int window_width() const { return window_width_; }
    [[nodiscard]] int window_height() const { return window_height_; }

    [[nodiscard]] const std::string& directory() const { return directory_; }

    [[nodiscard]] bool disable_vsync() const { return disable_vsync_; }

    [[nodiscard]] bool dump_first_video_frame() const { return dump_first_video_frame_; }

private:
    int font_size_ = 0;

    int window_width_ = 0;
    int window_height_ = 0;

    std::string directory_;

    bool disable_vsync_ = false;

    bool dump_first_video_frame_ = false;
};

}  // namespace video_trimmer::command_line

#pragma once

#include <optional>
#include <string>

#include "types.hpp"

namespace video_trimmer::command_line {

class CommandLine {
public:
    CommandLine(int argc, const char* argv[]);

    [[nodiscard]] int font_size() const { return font_size_; }
    [[nodiscard]] Size window_size() const { return window_size_; }

    [[nodiscard]] const std::string& directory() const { return directory_; }

    [[nodiscard]] bool disable_vsync() const { return disable_vsync_; }
    [[nodiscard]] bool dump_first_video_frame() const { return dump_first_video_frame_; }

private:
    int font_size_ = 20;
    Size window_size_{0, 0};

    std::string directory_;

    bool disable_vsync_ = false;
    bool dump_first_video_frame_ = false;
};

}  // namespace video_trimmer::command_line

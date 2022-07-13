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

private:
    int font_size_ = 0;

    int window_width_ = 0;
    int window_height_ = 0;

    std::string directory_;
};

}  // namespace video_trimmer::command_line

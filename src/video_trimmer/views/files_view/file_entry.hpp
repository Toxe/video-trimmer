#pragma once

#include <filesystem>
#include <string>

namespace video_trimmer::views::files_view {

class FileEntry {
    std::filesystem::path path_;

public:
    FileEntry(const std::filesystem::path& path);

    [[nodiscard]] std::string filename() const;
};

}  // namespace video_trimmer::views::files_view

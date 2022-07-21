#pragma once

#include <filesystem>
#include <string>

namespace video_trimmer::views::files_view {

class FileEntry {
public:
    FileEntry(const std::filesystem::path& path);

    [[nodiscard]] std::string full_filename() const;
    [[nodiscard]] std::string basename() const;

private:
    std::filesystem::path path_;
};

}  // namespace video_trimmer::views::files_view

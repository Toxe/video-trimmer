#pragma once

#include <filesystem>
#include <string>

class FileEntry {
    std::filesystem::path path_;

public:
    FileEntry(const std::filesystem::path& path);

    [[nodiscard]] std::string filename() const;
};

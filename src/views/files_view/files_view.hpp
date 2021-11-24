#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "directory_scanner.hpp"
#include "file_entry.hpp"

class FilesView {
    std::mutex mtx_;

    std::vector<FileEntry> files_;
    std::unique_ptr<DirectoryScanner> directory_scanner_;

    std::string directory_;

public:
    FilesView();

    void change_directory(const std::string& directory);
    void add_file(FileEntry file_entry);

    void render();
};

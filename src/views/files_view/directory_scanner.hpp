#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

class FilesView;

class DirectoryScanner {
    std::mutex mtx_;
    std::jthread thread_;

    std::atomic<bool> is_scanning_ = false;

public:
    [[nodiscard]] bool is_scanning() const { return is_scanning_; }

    void scan(FilesView* files_view, const std::string directory);
    void abort();
};

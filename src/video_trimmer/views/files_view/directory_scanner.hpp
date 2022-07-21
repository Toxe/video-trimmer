#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

namespace video_trimmer::views::files_view {

class FilesView;

class DirectoryScanner {
public:
    [[nodiscard]] bool is_scanning() const { return is_scanning_; }

    void scan(FilesView* files_view, const std::string directory);
    void abort();

private:
    std::mutex mtx_;
    std::jthread thread_;

    std::atomic<bool> is_scanning_ = false;
};

}  // namespace video_trimmer::views::files_view

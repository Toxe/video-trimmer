#pragma once

#include <atomic>
#include <mutex>
#include <string>
#include <thread>

namespace video_trimmer::views::files_view {

class FilesView;

class DirectoryScanner {
public:
    void scan(FilesView* files_view, std::string directory);
    void abort();

    [[nodiscard]] bool is_scanning() const { return is_scanning_; }
    [[nodiscard]] float scan_progress() const { return scan_progress_; }

private:
    std::mutex mtx_;
    std::jthread thread_;

    std::atomic<bool> is_scanning_ = false;
    std::atomic<float> scan_progress_ = 0.0f;
};

}  // namespace video_trimmer::views::files_view

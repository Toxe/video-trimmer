#include "directory_scanner.hpp"

#include <filesystem>

#include "file_entry.hpp"
#include "files_view.hpp"
#include "logger/logger.hpp"
#include "threaded_stream_reader/video_file.hpp"

void DirectoryScanner::scan(FilesView* files_view, const std::string directory)
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (is_scanning_) {
        log_warn("(DirectoryScanner) already scanning");
        return;
    }

    is_scanning_ = true;

    thread_ = std::jthread([this, files_view, directory](std::stop_token st) {
        log_trace("(DirectoryScanner) scan started");

        std::filesystem::path dir{directory};

        for (const auto& dir_entry : std::filesystem::directory_iterator{dir}) {
            if (dir_entry.is_regular_file()) {
                VideoFile video_file{dir_entry.path().string()};

                if (video_file.is_video()) {
                    FileEntry file_entry{dir_entry.path()};
                    files_view->add_file(std::move(file_entry));
                }
            }
        }

        log_trace("(DirectoryScanner) scan finished");

        is_scanning_ = false;
    });
}

void DirectoryScanner::abort()
{
    log_debug("(DirectoryScanner) abort scan");

    thread_.request_stop();
}

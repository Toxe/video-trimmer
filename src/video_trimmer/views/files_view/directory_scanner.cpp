#include "directory_scanner.hpp"

#include <filesystem>

#include "file_entry.hpp"
#include "files_view.hpp"
#include "video_trimmer/logger/logger.hpp"
#include "video_trimmer/video_reader/video_file/video_file.hpp"

namespace video_trimmer::views::files_view {

void DirectoryScanner::scan(FilesView* files_view, const std::string directory)
{
    std::lock_guard<std::mutex> lock(mtx_);

    if (is_scanning_) {
        video_trimmer::logger::log_warn("(DirectoryScanner) already scanning");
        return;
    }

    is_scanning_ = true;

    thread_ = std::jthread([this, files_view, directory](std::stop_token st) {
        video_trimmer::logger::log_trace("(DirectoryScanner) scan started");

        std::filesystem::path dir{directory};

        for (const auto& dir_entry : std::filesystem::directory_iterator{dir}) {
            if (dir_entry.is_regular_file()) {
                video_reader::video_file::VideoFile video_file{dir_entry.path().string()};

                if (video_file.is_video()) {
                    FileEntry file_entry{dir_entry.path()};
                    files_view->add_file(std::move(file_entry));
                }
            }
        }

        video_trimmer::logger::log_trace("(DirectoryScanner) scan finished");

        is_scanning_ = false;
    });
}

void DirectoryScanner::abort()
{
    video_trimmer::logger::log_debug("(DirectoryScanner) abort scan");

    thread_.request_stop();
}

}  // namespace video_trimmer::views::files_view

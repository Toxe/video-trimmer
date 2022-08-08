#include "directory_scanner.hpp"

#include <algorithm>
#include <filesystem>
#include <vector>

#include "file_entry.hpp"
#include "files_view.hpp"
#include "video_trimmer/logger/logger.hpp"
#include "video_trimmer/video_file/video_file.hpp"

namespace video_trimmer::views::files_view {

void DirectoryScanner::scan(FilesView* files_view, const std::string directory)
{
    if (is_scanning_) {
        video_trimmer::logger::log_warn("(DirectoryScanner) already scanning");
        return;
    }

    scan_progress_ = 0.0f;
    is_scanning_ = true;

    thread_ = std::jthread([this, files_view, directory](std::stop_token st) {
        video_trimmer::logger::log_trace("(DirectoryScanner) scan started");

        // list and sort all files
        std::filesystem::path dir{directory};
        std::vector<std::filesystem::path> files;

        for (const auto& dir_entry : std::filesystem::directory_iterator{dir})
            if (dir_entry.is_regular_file())
                files.push_back(dir_entry.path());

        std::sort(files.begin(), files.end());

        // analyze files and return all videos
        int scanned_files = 0;

        for (const auto& file : files) {
            if (st.stop_requested())
                break;

            video_file::VideoFile video_file{file.string()};

            if (video_file.is_video()) {
                FileEntry file_entry{video_file, file};
                files_view->add_file(std::move(file_entry));
            }

            scan_progress_ = static_cast<float>(++scanned_files) / static_cast<float>(files.size());
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

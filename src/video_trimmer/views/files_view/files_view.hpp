#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "directory_scanner.hpp"
#include "file_entry.hpp"

namespace video_trimmer::event_handler {
class EventHandler;
}

namespace video_trimmer::views::files_view {

class FilesView {
public:
    FilesView(event_handler::EventHandler& event_handler);

    void change_directory(const std::string& directory);
    void add_file(FileEntry file_entry);

    void render();

    [[nodiscard]] std::string selected_filename() const;

private:
    std::mutex mtx_;

    event_handler::EventHandler& event_handler_;

    std::vector<FileEntry> files_;
    std::unique_ptr<DirectoryScanner> directory_scanner_;

    std::string directory_;

    int selected_index_ = -1;

    void show_controls();
    void show_file_table();
    void show_tooltip(const FileEntry& file);
};

}  // namespace video_trimmer::views::files_view

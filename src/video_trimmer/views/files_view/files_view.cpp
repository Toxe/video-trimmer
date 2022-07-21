#include "files_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "directory_scanner.hpp"
#include "video_trimmer/event_handler/event_handler.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::views::files_view {

FilesView::FilesView(event_handler::EventHandler& event_handler)
    : event_handler_(event_handler)
{
    directory_scanner_ = std::make_unique<DirectoryScanner>();
}

void FilesView::change_directory(const std::string& directory)
{
    video_trimmer::logger::log_debug(fmt::format("(FilesView) change directory: {}", directory));

    if (directory_scanner_->is_scanning()) {
        video_trimmer::logger::log_warn("(FilesView) already scanning another directory");
        return;
    }

    std::lock_guard<std::mutex> lock(mtx_);

    directory_ = directory;
    files_.clear();
    selected_index_ = -1;

    directory_scanner_->scan(this, directory);
}

void FilesView::render()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("left pane");
    ImGui::BeginChild("files");

    ImGui::BeginListBox("##file list", ImVec2(-FLT_MIN, -FLT_MIN));

    if (directory_scanner_->is_scanning()) {
        ImGui::Text("scanning directory...");
        ImGui::SameLine();

        if (ImGui::Button("cancel"))
            directory_scanner_->abort();
    } else {
        if (ImGui::Button("rescan directory"))
            change_directory(directory_);
    }

    {
        std::lock_guard<std::mutex> lock(mtx_);
        int index = 0;

        for (auto& file : files_) {
            if (ImGui::Selectable(file.basename().c_str(), selected_index_ == index)) {
                selected_index_ = index;
                event_handler_.handle_event(event_handler::Event::OpenFile);
            }

            ++index;
        }
    }

    ImGui::EndListBox();

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

void FilesView::add_file(FileEntry file_entry)
{
    std::lock_guard<std::mutex> lock(mtx_);

    files_.push_back(std::move(file_entry));
}

std::string FilesView::selected_filename() const
{
    return files_.at(selected_index_).full_filename();
}

}  // namespace video_trimmer::views::files_view

#include "files_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "directory_scanner.hpp"
#include "video_trimmer/event_handler/event_handler.hpp"
#include "video_trimmer/logger/logger.hpp"
#include "video_trimmer/ui/colors/colors.hpp"

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

    show_controls();
    show_file_table();

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

void FilesView::show_controls()
{
    if (directory_scanner_->is_scanning()) {
        ImGui::Text("scanning directory...");
        ImGui::SameLine();

        if (ImGui::Button("cancel"))
            directory_scanner_->abort();
    } else {
        if (ImGui::Button("rescan directory"))
            change_directory(directory_);
    }
}

void FilesView::show_file_table()
{
    std::lock_guard<std::mutex> lock(mtx_);

    int index = 0;

    // little less horizontal and vertical padding
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {2.0f, 2.0f});

    if (ImGui::BeginTable("files_view_table", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBodyUntilResize)) {
        ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        for (auto& file : files_) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_Text, ui::colors::light_blue);

            if (ImGui::Selectable(file.basename().c_str(), selected_index_ == index, ImGuiSelectableFlags_SpanAllColumns)) {
                selected_index_ = index;
                event_handler_.handle_event(event_handler::Event::OpenFile);
            }

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
                show_tooltip(file);

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(fmt::format("{:>8} ", file.file_size()).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(file.video_duration().c_str());

            ++index;
        }

        ImGui::EndTable();

        ImGui::PopStyleVar();
    }
}

void FilesView::show_tooltip(const FileEntry& file)
{
    ImGui::BeginTooltip();

    ImGui::TextUnformatted(file.basename().c_str());
    ImGui::Separator();

    ImGui::TextUnformatted(file.video_format().c_str());
    ImGui::TextUnformatted(file.video_codec_name().c_str());
    ImGui::TextUnformatted(file.video_codec_additional_info().c_str());
    ImGui::NewLine();

    ImGui::TextUnformatted(fmt::format("Size: {}", file.file_size()).c_str());
    ImGui::TextUnformatted(fmt::format("Duration: {}", file.video_duration()).c_str());

    ImGui::EndTooltip();
}

}  // namespace video_trimmer::views::files_view

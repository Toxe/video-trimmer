#include "files_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "directory_scanner.hpp"
#include "lib/ImGuiFileDialog/ImGuiFileDialog.h"
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
        ImGui::TextUnformatted("scanning directory...");
        ImGui::SameLine();

        if (ImGui::Button("cancel"))
            directory_scanner_->abort();
    } else {
        if (ImGui::Button("rescan directory"))
            change_directory(directory_);

        ImGui::SameLine();

        show_change_directory_dialog();
    }
}

void FilesView::show_file_table()
{
    std::lock_guard<std::mutex> lock(mtx_);

    int index = 0;

    if (files_.empty() && !directory_scanner_->is_scanning()) {
        ImGui::TextUnformatted("no videos found");
        return;
    }

    // little less horizontal and vertical padding
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, {2.0f, 2.0f});

    if (ImGui::BeginTable("files_view_table", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoBordersInBodyUntilResize)) {
        ImGui::TableSetupColumn("File", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 95);  // TODO: column width is depending on font size
        ImGui::TableSetupColumn("Duration", ImGuiTableColumnFlags_WidthFixed, 77);
        ImGui::TableHeadersRow();

        for (auto& file : files_) {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();

            ImGui::PushStyleColor(ImGuiCol_Text, file.is_supported() ? ui::colors::light_blue : ui::colors::dark_grey);

            if (ImGui::Selectable(file.basename().c_str(), selected_index_ == index, ImGuiSelectableFlags_SpanAllColumns)) {
                selected_index_ = index;

                if (file.is_supported())
                    event_handler_.handle_event(event_handler::Event::OpenFile);
            }

            ImGui::PopStyleColor();

            if (ImGui::IsItemHovered())
                show_tooltip(file);

            if (!file.is_supported())
                ImGui::PushStyleColor(ImGuiCol_Text, ui::colors::dark_grey);

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(fmt::format("{:>9}", file.file_size()).c_str());

            ImGui::TableNextColumn();
            ImGui::TextUnformatted(file.video_duration().c_str());

            if (!file.is_supported())
                ImGui::PopStyleColor();

            ++index;
        }

        ImGui::EndTable();

        ImGui::PopStyleVar();
    }
}

void FilesView::show_tooltip(const FileEntry& file)
{
    ImGui::BeginTooltip();

    ImGui::TextColored(ui::colors::light_blue, file.basename().c_str());
    ImGui::Separator();

    ImGui::TextUnformatted(file.video_format().c_str());
    ImGui::TextUnformatted(file.video_codec_name().c_str());
    ImGui::TextUnformatted(file.video_codec_additional_info().c_str());
    ImGui::NewLine();

    ImGui::TextUnformatted(fmt::format("Size: {}", file.file_size()).c_str());
    ImGui::TextUnformatted(fmt::format("Duration: {}", file.video_duration()).c_str());

    if (!file.is_supported()) {
        ImGui::NewLine();
        ImGui::TextColored(ui::colors::red, file.not_supported_note().c_str());
    }

    ImGui::EndTooltip();
}

void FilesView::show_change_directory_dialog()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    const ImVec2 min_size{viewport->WorkSize.x / 2.0f, viewport->WorkSize.y / 2.0f};
    const ImVec2 max_size{viewport->WorkSize.x * 0.8f, viewport->WorkSize.y * 0.8f};

    // open "change directory" dialog
    if (ImGui::Button("change directory"))
        ImGuiFileDialog::Instance()->OpenDialog("ChooseDirDlgKey", "Choose a Directory", nullptr, directory_ + "/", 1, nullptr, ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_DontShowHiddenFiles | ImGuiFileDialogFlags_DisableCreateDirectoryButton);

    // display dialog
    if (ImGuiFileDialog::Instance()->Display("ChooseDirDlgKey", ImGuiWindowFlags_NoCollapse, min_size, max_size)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            const std::string directory = ImGuiFileDialog::Instance()->GetCurrentPath();
            video_trimmer::logger::log_debug(fmt::format("(FilesView) seleted directory: {}", directory));
            change_directory(directory);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

}  // namespace video_trimmer::views::files_view

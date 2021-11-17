#include "main_window_view.hpp"

#include <imgui.h>

#include "left_pane/left_pane.hpp"
#include "right_pane/right_pane.hpp"

MainWindowView::MainWindowView(const char* name) : View(name)
{
    left_pane_ = std::make_unique<LeftPane>();
    right_pane_ = std::make_unique<RightPane>();
}

void MainWindowView::render_ui()
{
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin(name(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);

//    render_left_pane(left_pane_width, elapsed_time, video_file);
    left_pane_->render_ui();

    ImGui::SameLine();

//    render_right_pane();
    right_pane_->render_ui();

    ImGui::End();
}

#include "imgui_helpers.hpp"

namespace video_trimmer::ui {

// Draw outlined text.
void imgui_text_outlined(const ImVec4& text_color, const ImVec4& outline_color, const std::string& text)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const float outline_size = 1.0f;

    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y)
            if (x != 0 && y != 0)
                draw_list->AddText(ImVec2(pos.x + static_cast<float>(x) * outline_size, pos.y + static_cast<float>(y) * outline_size), ImGui::GetColorU32(outline_color), text.c_str());

    draw_list->AddText(pos, ImGui::GetColorU32(text_color), text.c_str());

    ImGui::NewLine();
}

}  // namespace video_trimmer::ui

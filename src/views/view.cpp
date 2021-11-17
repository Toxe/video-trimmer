#include "view.hpp"

#include <fmt/core.h>
#include <imgui.h>

View::View(const char* name) : name_{name}
{
}

void View::render_content()
{
}

void View::render_ui()
{
}

void View::show_view_info()
{
    ImGui::Text(fmt::format("{} [{}x{}]", name(), ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
}

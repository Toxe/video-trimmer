#pragma once

#include <string>

#include "imgui.h"

namespace video_trimmer::ui {

void imgui_text_outlined(const ImVec4& text_color, const ImVec4& outline_color, const std::string& text);

}  // namespace video_trimmer::ui

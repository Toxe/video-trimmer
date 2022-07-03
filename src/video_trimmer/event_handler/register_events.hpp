#pragma once

#include "event_handler.hpp"
#include "video_trimmer/main_window/main_window.hpp"
#include "video_trimmer/ui/ui.hpp"

namespace video_trimmer::event_handler {

void register_events(EventHandler& event_handler, video_trimmer::main_window::MainWindow& window, video_trimmer::ui::UI& ui);

}  // namespace video_trimmer::event_handler

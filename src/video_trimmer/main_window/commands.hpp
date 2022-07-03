#pragma once

#include "main_window.hpp"
#include "video_trimmer/event_handler/command.hpp"

namespace video_trimmer::main_window {

extern video_trimmer::event_handler::Command CloseWindowCommand(MainWindow& window);
extern video_trimmer::event_handler::Command ResizedWindowCommand(MainWindow& window);

}  // namespace video_trimmer::main_window

#pragma once

#include "event_handler/command.hpp"
#include "main_window.hpp"

namespace video_trimmer::main_window {

extern video_trimmer::event_handler::Command CloseWindowCommand(video_trimmer::main_window::MainWindow& window);
extern video_trimmer::event_handler::Command ResizedWindowCommand(video_trimmer::main_window::MainWindow& window);

}  // namespace video_trimmer::main_window

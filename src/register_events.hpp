#pragma once

#include "event_handler/event_handler.hpp"
#include "main_window/main_window.hpp"
#include "ui/ui.hpp"

void register_events(video_trimmer::event_handler::EventHandler& event_handler, video_trimmer::main_window::MainWindow& window, video_trimmer::ui::UI& ui);

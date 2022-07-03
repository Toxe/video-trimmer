#pragma once

#include "event_handler/event_handler.hpp"
#include "main_window/main_window.hpp"
#include "ui/ui.h"

void register_events(video_trimmer::event_handler::EventHandler& event_handler, MainWindow& window, UI& ui);

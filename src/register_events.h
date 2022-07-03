#pragma once

#include "event_handler/event_handler.h"
#include "main_window/main_window.hpp"
#include "ui/ui.h"

void register_events(EventHandler& event_handler, MainWindow& window, UI& ui);

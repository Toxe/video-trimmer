#pragma once

#include "event_handler/command.h"
#include "main_window.hpp"

extern Command CloseWindowCommand(MainWindow& window);
extern Command ResizedWindowCommand(MainWindow& window);

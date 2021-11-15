#pragma once

#include "event_handler/command.h"
#include "window.h"

extern Command CloseWindowCommand(Window& window);
extern Command ResizedWindowCommand(Window& window);

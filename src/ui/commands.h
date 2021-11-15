#pragma once

#include "event_handler/command.h"
#include "logger/logger.hpp"
#include "ui.h"
#include "ui/ui.h"
#include "window/window.h"

Command ToggleHelpCommand(UI& ui)
{
    return [&] {
        log_debug("ToggleHelpCommand");
        ui.toggle_help();
    };
}

#include "commands.h"

#include "logger/logger.hpp"

Command ToggleHelpCommand(UI& ui)
{
    return [&] {
        log_debug("ToggleHelpCommand");
        ui.toggle_help();
    };
}

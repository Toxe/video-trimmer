#include "commands.h"

#include "logger/logger.hpp"

video_trimmer::event_handler::Command ToggleHelpCommand(UI& ui)
{
    return [&] {
        log_debug("ToggleHelpCommand");
        ui.toggle_help();
    };
}

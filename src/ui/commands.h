#pragma once

#include <spdlog/spdlog.h>

#include "ui.h"
#include "event_handler/command.h"
#include "ui/ui.h"
#include "window/window.h"

Command ToggleHelpCommand(UI& ui)
{
    return [&] {
        spdlog::debug("ToggleHelpCommand");
        ui.toggle_help();
    };
}

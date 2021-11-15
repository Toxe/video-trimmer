#pragma once

#include "event_handler/command.h"
#include "logger/logger.hpp"
#include "window.h"

Command CloseWindowCommand(Window& window)
{
    return [&] {
        log_debug("CloseWindowCommand");
        window.close();
    };
}

Command ResizedWindowCommand(Window& window)
{
    return [&] {
        log_debug("ResizedWindowCommand");
        window.resized_window();
    };
}

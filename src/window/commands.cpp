#include "commands.h"

#include "logger/logger.hpp"

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

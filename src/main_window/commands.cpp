#include "commands.h"

#include "logger/logger.hpp"

Command CloseWindowCommand(MainWindow& window)
{
    return [&] {
        log_debug("CloseWindowCommand");
        window.close();
    };
}

Command ResizedWindowCommand(MainWindow& window)
{
    return [&] {
        log_debug("ResizedWindowCommand");
        window.resized_window();
    };
}

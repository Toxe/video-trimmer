#include "commands.h"

#include "logger/logger.hpp"

video_trimmer::event_handler::Command CloseWindowCommand(MainWindow& window)
{
    return [&] {
        log_debug("CloseWindowCommand");
        window.close();
    };
}

video_trimmer::event_handler::Command ResizedWindowCommand(MainWindow& window)
{
    return [&] {
        log_debug("ResizedWindowCommand");
        window.resized_window();
    };
}

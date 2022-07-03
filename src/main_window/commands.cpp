#include "commands.h"

#include "logger/logger.hpp"

video_trimmer::event_handler::Command CloseWindowCommand(MainWindow& window)
{
    return [&] {
        video_trimmer::logger::log_debug("CloseWindowCommand");
        window.close();
    };
}

video_trimmer::event_handler::Command ResizedWindowCommand(MainWindow& window)
{
    return [&] {
        video_trimmer::logger::log_debug("ResizedWindowCommand");
        window.resized_window();
    };
}

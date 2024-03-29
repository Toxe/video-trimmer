#include "commands.hpp"

#include "../logger/logger.hpp"
#include "main_window.hpp"

namespace video_trimmer::main_window {

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

}  // namespace video_trimmer::main_window

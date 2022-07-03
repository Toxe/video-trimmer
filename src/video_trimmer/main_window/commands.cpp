#include "commands.hpp"

#include "main_window.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::main_window {

video_trimmer::event_handler::Command CloseWindowCommand(video_trimmer::main_window::MainWindow& window)
{
    return [&] {
        video_trimmer::logger::log_debug("CloseWindowCommand");
        window.close();
    };
}

video_trimmer::event_handler::Command ResizedWindowCommand(video_trimmer::main_window::MainWindow& window)
{
    return [&] {
        video_trimmer::logger::log_debug("ResizedWindowCommand");
        window.resized_window();
    };
}

}  // namespace video_trimmer::main_window

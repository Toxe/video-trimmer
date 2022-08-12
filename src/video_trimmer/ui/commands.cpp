#include "commands.hpp"

#include "../logger/logger.hpp"

namespace video_trimmer::ui {

video_trimmer::event_handler::Command ToggleHelpCommand(UI& ui)
{
    return [&] {
        video_trimmer::logger::log_debug("ToggleHelpCommand");
        ui.toggle_help();
    };
}

}  // namespace video_trimmer::ui

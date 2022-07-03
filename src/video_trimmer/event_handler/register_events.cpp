#include "register_events.hpp"

#include "events.hpp"
#include "video_trimmer/main_window/commands.hpp"
#include "video_trimmer/ui/commands.hpp"

namespace video_trimmer::event_handler {

void register_events(EventHandler& event_handler, video_trimmer::main_window::MainWindow& window, video_trimmer::ui::UI& ui)
{
    // MainWindow
    event_handler.set_command(Event::CloseWindow, video_trimmer::main_window::CloseWindowCommand(window));
    event_handler.set_command(Event::ResizedWindow, video_trimmer::main_window::ResizedWindowCommand(window));

    // UI
    event_handler.set_command(Event::ToggleHelp, video_trimmer::ui::ToggleHelpCommand(ui));
}

}  // namespace video_trimmer::event_handler

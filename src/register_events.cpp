#include "register_events.h"

#include "event_handler/events.hpp"
#include "main_window/commands.h"
#include "ui/commands.h"

void register_events(video_trimmer::event_handler::EventHandler& event_handler, MainWindow& window, UI& ui)
{
    // MainWindow
    event_handler.set_command(video_trimmer::event_handler::Event::CloseWindow, CloseWindowCommand(window));
    event_handler.set_command(video_trimmer::event_handler::Event::ResizedWindow, ResizedWindowCommand(window));

    // UI
    event_handler.set_command(video_trimmer::event_handler::Event::ToggleHelp, ToggleHelpCommand(ui));
}

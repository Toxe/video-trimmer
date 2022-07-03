#include "register_events.hpp"

#include "event_handler/events.hpp"
#include "main_window/commands.hpp"
#include "ui/commands.h"

void register_events(video_trimmer::event_handler::EventHandler& event_handler, video_trimmer::main_window::MainWindow& window, UI& ui)
{
    // MainWindow
    event_handler.set_command(video_trimmer::event_handler::Event::CloseWindow, video_trimmer::main_window::CloseWindowCommand(window));
    event_handler.set_command(video_trimmer::event_handler::Event::ResizedWindow, video_trimmer::main_window::ResizedWindowCommand(window));

    // UI
    event_handler.set_command(video_trimmer::event_handler::Event::ToggleHelp, ToggleHelpCommand(ui));
}

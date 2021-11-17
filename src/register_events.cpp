#include "register_events.h"

#include "event_handler/event_handler.h"
#include "main_window/commands.h"
#include "main_window/main_window.hpp"
#include "ui/commands.h"
#include "ui/ui.h"

void register_events(EventHandler& event_handler, MainWindow& window, UI& ui)
{
    // MainWindow
    event_handler.set_command(Event::CloseWindow, CloseWindowCommand(window));
    event_handler.set_command(Event::ResizedWindow, ResizedWindowCommand(window));

    // UI
    event_handler.set_command(Event::ToggleHelp, ToggleHelpCommand(ui));
}

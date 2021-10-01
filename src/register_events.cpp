#include "register_events.h"

#include "event_handler/event_handler.h"
#include "ui/ui.h"
#include "window/window.h"

#include "ui/commands.h"
#include "window/commands.h"

void register_events(EventHandler& event_handler, Window& window, UI& ui)
{
    // Window
    event_handler.set_command(Event::CloseWindow,   CloseWindowCommand(window));
    event_handler.set_command(Event::ResizedWindow, ResizedWindowCommand(window));

    // UI
    event_handler.set_command(Event::ToggleHelp, ToggleHelpCommand(ui));
}

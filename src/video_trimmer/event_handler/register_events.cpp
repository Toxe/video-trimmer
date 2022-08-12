#include "register_events.hpp"

#include "../main_window/commands.hpp"
#include "../main_window/main_window.hpp"
#include "../ui/commands.hpp"
#include "../ui/ui.hpp"
#include "../video_player/commands.hpp"
#include "../video_player/video_player.hpp"
#include "../views/files_view/files_view.hpp"
#include "events.hpp"

namespace video_trimmer::event_handler {

void register_events(EventHandler& event_handler, main_window::MainWindow& window, ui::UI& ui, video_player::VideoPlayer& video_player, views::files_view::FilesView& files_view)
{
    // MainWindow
    event_handler.set_command(Event::CloseWindow, main_window::CloseWindowCommand(window));
    event_handler.set_command(Event::ResizedWindow, main_window::ResizedWindowCommand(window));

    // UI
    event_handler.set_command(Event::ToggleHelp, ui::ToggleHelpCommand(ui));

    // VideoPlayer
    event_handler.set_command(Event::OpenFile, video_player::OpenFileCommand(video_player, window, files_view));
    event_handler.set_command(Event::PlaybackStart, video_player::PlaybackStartCommand(video_player));
    event_handler.set_command(Event::PlaybackTogglePause, video_player::PlaybackTogglePauseCommand(video_player));
    event_handler.set_command(Event::JumpBackward, video_player::JumpBackwardCommand(video_player));
    event_handler.set_command(Event::JumpForward, video_player::JumpForwardCommand(video_player));
}

}  // namespace video_trimmer::event_handler

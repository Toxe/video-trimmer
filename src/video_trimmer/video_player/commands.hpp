#pragma once

#include "../event_handler/command.hpp"
#include "../main_window/main_window.hpp"
#include "../views/files_view/files_view.hpp"
#include "video_player.hpp"

namespace video_trimmer::video_player {

extern event_handler::Command OpenFileCommand(VideoPlayer& video_player, main_window::MainWindow& window, views::files_view::FilesView& files_view);
extern event_handler::Command PlaybackStartCommand(VideoPlayer& video_player);
extern event_handler::Command PlaybackTogglePauseCommand(VideoPlayer& video_player);
extern event_handler::Command JumpBackwardShortCommand(VideoPlayer& video_player);
extern event_handler::Command JumpBackwardLongCommand(VideoPlayer& video_player);
extern event_handler::Command JumpForwardShortCommand(VideoPlayer& video_player);
extern event_handler::Command JumpForwardLongCommand(VideoPlayer& video_player);

}  // namespace video_trimmer::video_player

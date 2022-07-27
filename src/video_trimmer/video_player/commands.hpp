#pragma once

#include "video_player.hpp"
#include "video_trimmer/event_handler/command.hpp"
#include "video_trimmer/main_window/main_window.hpp"
#include "video_trimmer/views/files_view/files_view.hpp"

namespace video_trimmer::video_player {

extern event_handler::Command PlaybackStartCommand(VideoPlayer& video_player);
extern event_handler::Command PlaybackTogglePauseCommand(VideoPlayer& video_player);
extern event_handler::Command OpenFileCommand(VideoPlayer& video_player, main_window::MainWindow& window, views::files_view::FilesView& files_view);

}  // namespace video_trimmer::video_player

#pragma once

#include "event_handler.hpp"

namespace video_trimmer::main_window {
class MainWindow;
}  // namespace video_trimmer::main_window

namespace video_trimmer::ui {
class UI;
}  // namespace video_trimmer::ui

namespace video_trimmer::video_player {
class VideoPlayer;
}  // namespace video_trimmer::video_player

namespace video_trimmer::views::files_view {
class FilesView;
}  // namespace video_trimmer::views::files_view

namespace video_trimmer::event_handler {

void register_events(EventHandler& event_handler, main_window::MainWindow& window, ui::UI& ui, video_player::VideoPlayer& video_player, views::files_view::FilesView& files_view);

}  // namespace video_trimmer::event_handler

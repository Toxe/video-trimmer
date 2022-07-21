#pragma once

#include "video_player.hpp"
#include "video_trimmer/event_handler/command.hpp"

namespace video_trimmer::video_player {

extern event_handler::Command PlaybackStartCommand(VideoPlayer& video_player);
extern event_handler::Command PlaybackTogglePauseCommand(VideoPlayer& video_player);

}  // namespace video_trimmer::video_player

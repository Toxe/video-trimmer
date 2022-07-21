#include "commands.hpp"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_player {

event_handler::Command PlaybackStartCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("PlaybackStartCommand");
        video_player.start();
    };
}

event_handler::Command PlaybackTogglePauseCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("PlaybackTogglePauseCommand");
        video_player.toggle_pause();
    };
}

}  // namespace video_trimmer::video_player

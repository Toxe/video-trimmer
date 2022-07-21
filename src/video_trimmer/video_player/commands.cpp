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

event_handler::Command OpenFileCommand(VideoPlayer& video_player, views::files_view::FilesView& files_view)
{
    return [&] {
        logger::log_debug("OpenFileCommand");

        video_player.close_file();
        video_player.open_file(files_view.selected_filename());
        video_player.start();
    };
}

}  // namespace video_trimmer::video_player

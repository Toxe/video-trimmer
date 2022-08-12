#include "commands.hpp"

#include "../logger/logger.hpp"

namespace video_trimmer::video_player {

event_handler::Command OpenFileCommand(VideoPlayer& video_player, main_window::MainWindow& window, views::files_view::FilesView& files_view)
{
    return [&] {
        logger::log_debug("OpenFileCommand");

        video_player.close_file();

        // immediately create a compatible render texture instead of waiting for the first frame
        if (video_player.open_file(files_view.selected_filename()))
            window.video_view().create_compatible_render_texture_if_necessary(window.graphics(), video_player.video_file()->video_codec_context()->size(), video_player.video_file()->video_codec_context()->pixel_format());

        video_player.start();
    };
}

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

event_handler::Command JumpBackwardCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpBackwardCommand");
        video_player.jump_backward();
    };
}

event_handler::Command JumpForwardCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpForwardCommand");
        video_player.jump_forward();
    };
}

}  // namespace video_trimmer::video_player

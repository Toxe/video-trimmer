#include "commands.hpp"

#include "../logger/logger.hpp"

namespace video_trimmer::video_player {

constexpr std::chrono::seconds short_jump_seconds{5};
constexpr std::chrono::seconds long_jump_seconds{60};

event_handler::Command OpenFileCommand(VideoPlayer& video_player, main_window::MainWindow& window, views::files_view::FilesView& files_view)
{
    return [&] {
        logger::log_debug("OpenFileCommand");

        const auto now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

        video_player.close_file();
        video_player.play_file(std::make_unique<video_file::VideoFile>(files_view.selected_filename()), now);

        // immediately create a compatible render texture instead of waiting for the first frame
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

event_handler::Command JumpBackwardShortCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpBackwardShortCommand");
        video_player.jump_backward(short_jump_seconds);
    };
}

event_handler::Command JumpBackwardLongCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpBackwardLongCommand");
        video_player.jump_backward(long_jump_seconds);
    };
}

event_handler::Command JumpForwardShortCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpForwardShortCommand");
        video_player.jump_forward(short_jump_seconds);
    };
}

event_handler::Command JumpForwardLongCommand(VideoPlayer& video_player)
{
    return [&] {
        logger::log_debug("JumpForwardLongCommand");
        video_player.jump_forward(long_jump_seconds);
    };
}

}  // namespace video_trimmer::video_player

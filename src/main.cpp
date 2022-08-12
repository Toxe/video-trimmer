#include "video_trimmer/app/app.hpp"
#include "video_trimmer/command_line/command_line.hpp"
#include "video_trimmer/event_handler/event_handler.hpp"
#include "video_trimmer/event_handler/register_events.hpp"
#include "video_trimmer/main_window/main_window.hpp"
#include "video_trimmer/ui/ui.hpp"
#include "video_trimmer/video_player/video_player.hpp"
#include "video_trimmer/views/additional_info_view/additional_info_view.hpp"
#include "video_trimmer/views/files_view/files_view.hpp"
#include "video_trimmer/views/playback_controls_view/playback_controls_view.hpp"
#include "video_trimmer/views/trim_controls_view/trim_controls_view.hpp"

int main(int argc, char* argv[])
{
    video_trimmer::command_line::CommandLine cli(argc, argv);

    video_trimmer::event_handler::EventHandler event_handler;

    video_trimmer::app::App app;
    video_trimmer::ui::UI ui(event_handler);
    video_trimmer::main_window::MainWindow window(cli.window_size(), cli.font_size(), cli.disable_vsync());
    video_trimmer::video_player::VideoPlayer video_player(cli.dump_first_video_frame());

    video_trimmer::views::additional_info_view::AdditionalInfoView additional_info_view;
    video_trimmer::views::playback_controls_view::PlaybackControlsView playback_controls_view(event_handler);
    video_trimmer::views::trim_controls_view::TrimControlsView trim_controls_view;
    video_trimmer::views::files_view::FilesView files_view(event_handler);

    files_view.change_directory(cli.directory());

    video_trimmer::event_handler::register_events(event_handler, window, ui, video_player, files_view);

    while (window.is_open()) {
        app.begin_frame();
        window.begin_frame();

        event_handler.poll_events();

        if (window.is_open()) {
            auto video_frame = video_player.next_frame();

            if (video_frame && video_player.is_playing())
                window.show_video_frame(std::move(video_frame));

            ui.render();
            additional_info_view.render(app.previous_frame_time(), video_player.video_file());
            files_view.render();
            playback_controls_view.render(video_player);
            trim_controls_view.render();

            const video_trimmer::clock::Duration frame_time = window.render(app.frame_time_clock());
            app.set_previous_frame_time(frame_time);
        }
    }

    video_player.close_file();

    return 0;
}

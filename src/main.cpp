#include "app/app.h"
#include "command_line/command_line.h"
#include "event_handler/event_handler.h"
#include "main_window/main_window.hpp"
#include "register_events.h"
#include "threaded_stream_reader/video_frame/video_frame.hpp"
#include "ui/ui.h"
#include "video_player/video_player.hpp"
#include "views/additional_info_view/additional_info_view.hpp"
#include "views/files_view/files_view.hpp"
#include "views/playback_controls_view/playback_controls_view.hpp"
#include "views/trim_controls_view/trim_controls_view.hpp"

int main(int argc, char* argv[])
{
    CommandLine cli(argc, argv);

    App app;
    UI ui;
    MainWindow window(cli);
    AdditionalInfoView additional_info_view(cli);
    FilesView files_view;
    PlaybackControlsView playback_controls_view;
    TrimControlsView trim_controls_view;

    EventHandler event_handler;
    register_events(event_handler, window, ui);
    ui.set_event_handler(&event_handler);

    VideoPlayer video_player;
    video_player.open_file("video1.mp4");
    video_player.start();

    while (window.is_open()) {
        app.next_frame();
        window.next_frame(app.elapsed_time());

        event_handler.poll_events(window.window());

        if (window.is_open()) {
            video_player.update();

            ui.render();
            additional_info_view.render(app.elapsed_time(), video_player.video_file());
            files_view.render();
            playback_controls_view.render(video_player.playback_position());
            trim_controls_view.render();

            if (video_player.is_playing()) {
                auto video_frame = video_player.next_frame();

                window.render(video_player.finished_video_frame_queue_size(), video_player.video_frame_scaler_queue_size(), video_frame.get());
            } else {
                window.render(video_player.finished_video_frame_queue_size(), video_player.video_frame_scaler_queue_size(), nullptr);
            }

            video_player.change_scaling_dimensions(window.video_view().size());

            // if (received_first_real_frame && frames_available == 0 && video_content_provider.has_finished())
            //     break;
        }
    }

    video_player.stop();
    video_player.close_file();
}

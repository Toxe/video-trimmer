#include "app/app.h"
#include "command_line/command_line.h"
#include "event_handler/event_handler.h"
#include "register_events.h"
#include "ui/ui.h"
#include "video/video_file.hpp"
#include "video/video_content_provider.hpp"
#include "window/window.h"

int main(int argc, char* argv[])
{
    CommandLine cli(argc, argv);

    VideoFile video_file(cli.video_filename());
    VideoContentProvider video_content_provider(video_file.open_stream());

    App app;
    UI ui(cli);
    Window window(cli);

    EventHandler event_handler;
    register_events(event_handler, window, ui);
    ui.set_event_handler(&event_handler);

    while (window.is_open()) {
        app.next_frame();
        window.next_frame(app.elapsed_time());

        event_handler.poll_events(window.window());

        if (window.is_open()) {
            ui.render(app.elapsed_time(), video_file);
            window.render(ui.video_view_position(), ui.video_view_size(), video_content_provider);
        }

        if (window.is_open())
            video_content_provider.read(ui.video_view_size());
    }
}

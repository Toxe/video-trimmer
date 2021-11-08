#include <chrono>

#include <fmt/core.h>

#include "app/app.h"
#include "command_line/command_line.h"
#include "event_handler/event_handler.h"
#include "logger/logger.hpp"
#include "register_events.h"
#include "threaded_stream_reader/factory/ffmpeg_factory.hpp"
#include "threaded_stream_reader/video_content_provider/video_content_provider.hpp"
#include "threaded_stream_reader/video_file.hpp"
#include "threaded_stream_reader/video_frame/video_frame.hpp"
#include "ui/ui.h"
#include "window/window.h"

int main(int argc, char* argv[])
{
    CommandLine cli(argc, argv);

    const auto factory = std::make_unique<FFmpegFactory>();

    VideoFile video_file(cli.video_filename(), factory.get());
    VideoContentProvider video_content_provider(factory.get(), video_file, 1396, 806);
    video_content_provider.run();

    App app;
    UI ui(cli);
    Window window(cli);

    EventHandler event_handler;
    register_events(event_handler, window, ui);
    ui.set_event_handler(&event_handler);

    // begin playback
    auto playback_begin = std::chrono::steady_clock::now();
    bool received_first_real_frame = false;

    while (window.is_open()) {
        app.next_frame();
        window.next_frame(app.elapsed_time());

        event_handler.poll_events(window.window());

        if (window.is_open()) {
            // current position in playback
            if (!received_first_real_frame)
                playback_begin = std::chrono::steady_clock::now();

            const std::chrono::duration<double> playback_position = std::chrono::steady_clock::now() - playback_begin;

            const auto t1 = std::chrono::high_resolution_clock::now();
            auto [video_frame, frames_available] = video_content_provider.next_frame(playback_position.count());
            const auto t2 = std::chrono::high_resolution_clock::now();
            const auto ms = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

            if (video_frame) {
                log_trace(fmt::format("(main) playback_position={:.4f}, found frame: {} ({} more frames available), waited for {}us", playback_position.count(), video_frame->print(), frames_available, ms.count()));

                if (!received_first_real_frame) {
                    log_debug("(main) received first frame, begin playback");
                    received_first_real_frame = true;
                }

                ui.render(app.elapsed_time(), video_file);
                window.render(ui.video_view_position(), ui.video_view_size(), video_frame.get());
            } else {
                ui.render(app.elapsed_time(), video_file);
                window.render(ui.video_view_position(), ui.video_view_size(), nullptr);
            }

            if (received_first_real_frame && frames_available == 0 && video_content_provider.has_finished())
                break;
        }
    }

    video_content_provider.stop();
}

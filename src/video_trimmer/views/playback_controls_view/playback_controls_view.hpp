#pragma once

namespace video_trimmer::video_player {
class VideoPlayer;
}

namespace video_trimmer::event_handler {
class EventHandler;
}

namespace video_trimmer::views::playback_controls_view {

class PlaybackControlsView {
public:
    void render(video_player::VideoPlayer& video_player);

    void set_event_handler(event_handler::EventHandler* event_handler) { event_handler_ = event_handler; };

private:
    event_handler::EventHandler* event_handler_ = nullptr;
};

}  // namespace video_trimmer::views::playback_controls_view

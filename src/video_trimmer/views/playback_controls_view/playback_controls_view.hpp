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
    explicit PlaybackControlsView(event_handler::EventHandler& event_handler);

    void render(video_player::VideoPlayer& video_player);

private:
    event_handler::EventHandler& event_handler_;
};

}  // namespace video_trimmer::views::playback_controls_view

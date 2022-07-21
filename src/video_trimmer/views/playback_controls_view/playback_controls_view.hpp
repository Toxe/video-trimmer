#pragma once

namespace video_trimmer::video_player {
class VideoPlayer;
}

namespace video_trimmer::views::playback_controls_view {

class PlaybackControlsView {
public:
    void render(video_player::VideoPlayer& video_player);
};

}  // namespace video_trimmer::views::playback_controls_view

#pragma once

class PlaybackControlsView {
public:
    void render(double playback_position, int frames_available, int finished_video_frame_queue_size, int video_frame_scaler_queue_size);
};

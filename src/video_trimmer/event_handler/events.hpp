#pragma once

namespace video_trimmer::event_handler {

enum class Event {
    CloseWindow,
    ResizedWindow,
    ToggleHelp,
    PlaybackStart,
    PlaybackTogglePause,
    OpenFile,
};

}  // namespace video_trimmer::event_handler

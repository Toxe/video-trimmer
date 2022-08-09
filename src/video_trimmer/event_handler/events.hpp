#pragma once

namespace video_trimmer::event_handler {

enum class Event {
    CloseWindow,
    ResizedWindow,
    ToggleHelp,
    OpenFile,
    PlaybackStart,
    PlaybackTogglePause,
    JumpBackward,
    JumpForward,
};

}  // namespace video_trimmer::event_handler

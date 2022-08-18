#pragma once

namespace video_trimmer::event_handler {

enum class Event {
    CloseWindow,
    ResizedWindow,
    ToggleHelp,
    OpenFile,
    PlaybackStart,
    PlaybackTogglePause,
    JumpBackwardShort,
    JumpBackwardLong,
    JumpForwardShort,
    JumpForwardLong,
};

}  // namespace video_trimmer::event_handler

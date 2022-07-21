#pragma once

#include <string>

#include "imgui.h"

namespace video_trimmer::event_handler {
class EventHandler;
}

namespace video_trimmer::ui {

class UI {
public:
    explicit UI(event_handler::EventHandler& event_handler);

    void render();

    void toggle_help() { show_help_ = !show_help_; };

private:
    const char* main_window_title_ = "Video Trimmer";
    const char* help_window_title_ = "Help";

    event_handler::EventHandler& event_handler_;

    ImVec2 main_window_size_;

    bool show_help_ = false;

    void help(const std::string& text);

    void render_main_window();
    void render_help_window();

    void setup_left_pane(float pane_width);
    void setup_right_pane();

    void setup_files_view(float pane_height);
    void setup_additional_info_view(float pane_height);
    void setup_video_view(float pane_height);
    void setup_playback_controls_pane(float pane_height);
    void setup_trim_controls_pane(float pane_height);
};

}  // namespace video_trimmer::ui

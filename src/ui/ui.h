#pragma once

#include <string>
#include <vector>

#include <imgui.h>

#include "types.h"

class EventHandler;

class UI {
    const char* main_window_title_ = "Video Trimmer";
    const char* help_window_title_ = "Help";

    EventHandler* event_handler_ = nullptr;

    ImVec2 main_window_size_;
    ImVec2 video_view_position_;
    ImVec2 video_view_size_;

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

public:
    void render();

    void toggle_help() { show_help_ = !show_help_; };

    void set_event_handler(EventHandler* event_handler) { event_handler_ = event_handler; };

    [[nodiscard]] ImagePosition video_view_position() const;
    [[nodiscard]] ImageSize video_view_size() const;
};

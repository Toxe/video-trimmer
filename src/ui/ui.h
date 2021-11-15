#pragma once

#include <string>
#include <vector>

#include <imgui.h>

#include "types.h"
#include "views/fps_view.h"
#include "views/video_file_info_view.hpp"

class CommandLine;
class Duration;
class EventHandler;
class VideoFile;

class UI {
    const char* main_window_title_ = "Video Trimmer";
    const char* help_window_title_ = "Help";

    EventHandler* event_handler_ = nullptr;

    ImVec2 main_window_size_;
    ImVec2 video_view_position_;
    ImVec2 video_view_size_;

    float font_size_;

    bool show_help_ = false;

    FPSView fps_view_;
    VideoFileInfoView video_file_info_view_;

    void help(const std::string& text);

    void render_main_window(const Duration elapsed_time, const VideoFile& video_file);
    void render_help_window();

    void render_left_pane(const float pane_width, const Duration elapsed_time, const VideoFile& video_file);
    void render_right_pane();

    void render_files_pane(const float pane_height);
    void render_additional_info_pane(const float pane_height, const Duration elapsed_time, const VideoFile& video_file);
    void render_video_pane(const float pane_height);
    void render_playback_controls_pane(const float pane_height);
    void render_trim_controls_pane(const float pane_height);

public:
    UI(const CommandLine& cli);

    void render(const Duration elapsed_time, const VideoFile& video_file);

    void toggle_help() { show_help_ = !show_help_; };

    void set_event_handler(EventHandler* event_handler) { event_handler_ = event_handler; };

    ImagePosition video_view_position() const;
    ImageSize video_view_size() const;
};

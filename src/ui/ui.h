#pragma once

#include <string>
#include <vector>

#include <imgui.h>

#include "input_value.h"
#include "types.h"
#include "event_handler/event_handler.h"

class Duration;
class CommandLine;

class UI {
    const char* main_window_title_ = "Video Trimmer";
    const char* help_window_title_ = "Help";

    EventHandler* event_handler_ = nullptr;

    ImVec2 main_window_size_;

    float font_size_;

    bool show_help_ = false;

    void help(const std::string& text);
    bool input_int(const char* label, InputValue<int>& value, const int small_inc, const int big_inc, const int min, const int max);
    bool input_double(const char* label, InputValue<double>& value, const double small_inc, const double big_inc, const double min, const double max);

    void render_main_window(const Duration elapsed_time);
    void render_help_window();
    void render_interface_hidden_hint_window();

public:
    UI(const CommandLine& cli);

    void render(const Duration elapsed_time);

    void toggle_help() { show_help_ = !show_help_; };

    void set_event_handler(EventHandler* event_handler) { event_handler_ = event_handler; };
};

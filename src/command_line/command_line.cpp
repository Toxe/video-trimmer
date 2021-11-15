#include "command_line.h"

#include <cstdlib>
#include <filesystem>
#include <string>

#include <CLI/App.hpp>
#include <CLI/Config.hpp>
#include <CLI/Formatter.hpp>
#include <fmt/core.h>

#include "logger/logger.hpp"

CommandLine::CommandLine(int argc, char* argv[])
{
    const auto description = "Video Trimmer";
    int log_level_flag = 0;

    default_window_video_mode_ = default_video_mode();

    font_size_ = default_font_size();
    window_width_ = default_window_video_mode_.width;
    window_height_ = default_window_video_mode_.height;

    CLI::App app{description};
    app.add_flag("-v", log_level_flag, "log level (-v: INFO, -vv: DEBUG, -vvv: TRACE)");
    app.add_option("--font-size", font_size_, fmt::format("UI font size in pixels (default: {})", font_size_))->check(CLI::PositiveNumber);
    app.add_option("video filename", video_filename_, "name of video file to load")->required();
    auto opt_width = app.add_option("--width", window_width_, fmt::format("window width (default: {})", window_width_));
    auto opt_height = app.add_option("--height", window_height_, fmt::format("window height (default: {})", window_height_));

    opt_width->check(CLI::PositiveNumber)->needs(opt_height);
    opt_height->check(CLI::PositiveNumber)->needs(opt_width);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& error) {
        show_usage_and_exit(app, nullptr, error);
    }

    default_window_video_mode_.width = window_width_;
    default_window_video_mode_.height = window_height_;
    video_mode_ = default_window_video_mode_;

    LogLevel log_level = LogLevel::warn;

    switch (log_level_flag) {
        case  1: log_level = LogLevel::info;   break;
        case  2: log_level = LogLevel::debug;  break;
        case  3: log_level = LogLevel::trace;  break;
        default: log_level = LogLevel::warn;
    }

    log_init(log_level);
    log_debug(fmt::format("command line option video filename: {}", video_filename_));
    log_debug(fmt::format("command line option --font-size: {}", font_size_));
    log_debug(fmt::format("command line option --width: {}", window_width_));
    log_debug(fmt::format("command line option --height: {}", window_height_));

    if (!std::filesystem::exists(video_filename_))
        show_usage_and_exit(app, "file not found", {});
}

void CommandLine::show_usage_and_exit(const CLI::App& app, const char* error_message = nullptr, const std::optional<CLI::ParseError>& error = {})
{
    if (error_message)
        fmt::print("\n{}\n", error_message);

    std::exit(error ? app.exit(error.value()) : 0);
}

sf::VideoMode CommandLine::default_video_mode() const
{
    // init window at 50% desktop height and 16:9 aspect ratio
    const auto desktop = sf::VideoMode::getDesktopMode();
    const unsigned int height = desktop.height / 2;
    const unsigned int width = 16 * height / 9;
    return sf::VideoMode{width, height};
}

int CommandLine::default_font_size() const
{
    return static_cast<int>(sf::VideoMode::getDesktopMode().height) / 96;
}

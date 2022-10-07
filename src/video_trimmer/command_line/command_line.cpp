#include "command_line.hpp"

#include <cstdlib>
#include <filesystem>
#include <optional>

#include "CLI/App.hpp"
#include "CLI/Config.hpp"
#include "CLI/Formatter.hpp"
#include "fmt/core.h"

#include "../logger/logger.hpp"

namespace video_trimmer::command_line {

[[noreturn]] void show_usage_and_exit(const CLI::App& app, const char* error_message = nullptr, const std::optional<CLI::ParseError>& error = {})
{
    if (error_message)
        fmt::print("\n{}\n", error_message);

    std::exit(error ? app.exit(error.value()) : 0);
}

CommandLine::CommandLine(int argc, char* argv[])
{
    const char* description = "Video Trimmer";
    int log_level_flag = 0;

    directory_ = std::filesystem::current_path().string();

    CLI::App app{description};
    app.add_flag("-d,--dump", dump_first_video_frame_, fmt::format("dump first decoded video frame to file (default: {})", dump_first_video_frame_));
    app.add_flag("-v", log_level_flag, "log level (-v: INFO, -vv: DEBUG, -vvv: TRACE)");
    app.add_flag("--no-vsync", disable_vsync_, "disable vsync (vsync is enabled by default)");
    app.add_option("directory", directory_, "video directory (default: current directory)");
    app.add_option("--font-size", font_size_, fmt::format("UI font size in pixels (default: {})", font_size_));
    auto* opt_width = app.add_option("--width", window_size_.width, "window width");
    auto* opt_height = app.add_option("--height", window_size_.height, "window height");

    opt_width->check(CLI::PositiveNumber)->needs(opt_height);
    opt_height->check(CLI::PositiveNumber)->needs(opt_width);

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& error) {
        show_usage_and_exit(app, nullptr, error);
    }

    logger::LogLevel log_level = logger::LogLevel::warn;

    switch (log_level_flag) {
        case 1: log_level = logger::LogLevel::info; break;
        case 2: log_level = logger::LogLevel::debug; break;
        case 3: log_level = logger::LogLevel::trace; break;
        default:
            log_level = logger::LogLevel::warn;
    }

    logger::log_init(log_level);
    logger::log_debug(fmt::format("command line option directory: {}", directory_));
    logger::log_debug(fmt::format("command line option --font-size: {}", font_size_));
    logger::log_debug(fmt::format("command line option --width: {}", window_size_.width));
    logger::log_debug(fmt::format("command line option --height: {}", window_size_.height));
    logger::log_debug(fmt::format("command line option -d: {}", dump_first_video_frame_));
    logger::log_debug(fmt::format("command line option --no-vsync: {}", disable_vsync_));

    if (!std::filesystem::exists(directory_))
        show_usage_and_exit(app, fmt::format("directory not found: {}", directory_).c_str(), {});

    if (!std::filesystem::is_directory(directory_))
        show_usage_and_exit(app, fmt::format("not a directory: {}", directory_).c_str(), {});
}

}  // namespace video_trimmer::command_line

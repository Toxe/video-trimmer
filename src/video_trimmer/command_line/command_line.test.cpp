#include "catch2/catch_test_macros.hpp"

#include <array>
#include <filesystem>

#include "command_line.hpp"

namespace video_trimmer::command_line {

TEST_CASE("command_line::CommandLine")
{
    SECTION("can set directory")
    {
        const auto directory = std::filesystem::current_path().parent_path().string();

        auto args = std::to_array({"video_trimmer", directory.c_str()});
        const CommandLine cli(args);

        CHECK(cli.directory() == directory);
    }

    SECTION("can set window width and height")
    {
        auto args = std::to_array({"video_trimmer", "--width", "320", "--height", "240"});
        const CommandLine cli(args);

        CHECK(cli.window_size().width == 320);
        CHECK(cli.window_size().height == 240);
    }

    SECTION("default font size is 20")
    {
        auto args = std::to_array({"video_trimmer"});
        const CommandLine cli(args);

        CHECK(cli.font_size() == 20);
    }

    SECTION("can set font size with --font-size")
    {
        auto args = std::to_array({"video_trimmer", "--font-size", "15"});
        const CommandLine cli(args);

        CHECK(cli.font_size() == 15);
    }

    SECTION("default disable_vsync flag is false")
    {
        auto args = std::to_array({"video_trimmer"});
        const CommandLine cli(args);

        CHECK(cli.disable_vsync() == false);
    }

    SECTION("can set disable_vsync flag with --no-vsync")
    {
        auto args = std::to_array({"video_trimmer", "--no-vsync"});
        const CommandLine cli(args);

        CHECK(cli.disable_vsync() == true);
    }

    SECTION("default dump_first_video_frame flag is false")
    {
        auto args = std::to_array({"video_trimmer"});
        const CommandLine cli(args);

        CHECK(cli.dump_first_video_frame() == false);
    }

    SECTION("can set dump_first_video_frame flag with -d")
    {
        auto args = std::to_array({"video_trimmer", "-d"});
        const CommandLine cli(args);

        CHECK(cli.dump_first_video_frame() == true);
    }

    SECTION("can set dump_first_video_frame flag with --dump")
    {
        auto args = std::to_array({"video_trimmer", "--dump"});
        const CommandLine cli(args);

        CHECK(cli.dump_first_video_frame() == true);
    }
}

}  // namespace video_trimmer::command_line

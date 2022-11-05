#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

#include "fmt/core.h"

#include "fmt.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::clock {

TEST_CASE("clock::fmt")
{
    SECTION("FrameDuration")
    {
        CHECK_THAT(fmt::format("{}", FrameDuration{16667us}), Catch::Matchers::Equals("(frame duration: 0.017s | 16667µs)"));
    }

    SECTION("PlaybackPosition")
    {
        CHECK_THAT(fmt::format("{}", PlaybackPosition{16667us}), Catch::Matchers::Equals("(@ 0.017s | 16667µs)"));
    }

    SECTION("PlaybackTimePoint")
    {
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> tp{16667us};

        CHECK_THAT(fmt::format("{}", PlaybackTimePoint{tp}), Catch::Matchers::Equals("[= 0.017s | 16667µs]"));
    }

    SECTION("VideoDuration")
    {
        CHECK_THAT(fmt::format("{}", VideoDuration{16667us}), Catch::Matchers::Equals("(video duration: 0.017s | 16667µs)"));
    }
}

}  // namespace video_trimmer::clock

#include "catch2/catch_test_macros.hpp"

#include "playback_clock.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::clock {

TEST_CASE("clock::PlaybackClock")
{
    SECTION("construction")
    {
        const PlaybackClock clock;

        CHECK(clock.now().microseconds() >= 0us);
    }
}

}  // namespace video_trimmer::clock

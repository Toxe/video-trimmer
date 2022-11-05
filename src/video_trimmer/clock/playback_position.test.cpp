#include <cmath>

#include "catch2/catch_test_macros.hpp"

#include "frame_duration.hpp"
#include "playback_position.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::clock {

TEST_CASE("clock::PlaybackPosition")
{
    SECTION("construction")
    {
        const PlaybackPosition pos1{};
        const PlaybackPosition pos2{10us};
        const PlaybackPosition pos3 = PlaybackPosition{20us};
        const PlaybackPosition pos4{MicrosecondDuration{30us}};

        CHECK(pos1.microseconds() == 0us);
        CHECK(pos2.microseconds() == 10us);
        CHECK(pos3.microseconds() == 20us);
        CHECK(pos4.microseconds() == 30us);
    }

    SECTION("assignment")
    {
        PlaybackPosition pos1;
        PlaybackPosition pos2;

        pos1 = PlaybackPosition{10us};
        pos2 = 20us;

        CHECK(pos1.microseconds() == 10us);
        CHECK(pos2.microseconds() == 20us);
    }

    SECTION("comparison")
    {
        const PlaybackPosition pos1{100us};
        const PlaybackPosition pos2{200us};

        CHECK(pos1 < pos2);
        CHECK(pos2 > pos1);

        CHECK((pos1 == pos2) == false);
        CHECK(pos1 == PlaybackPosition{100us});

        CHECK(pos1 == 100us);
    }

    SECTION("addition")
    {
        const PlaybackPosition pos1{100us};
        const PlaybackPosition pos2{200us};

        PlaybackPosition pos;

        pos = pos1 + pos2;
        CHECK(pos == 300us);

        pos = pos1 + 300us;
        CHECK(pos == 400us);

        pos = pos1 + PlaybackPosition{100us};
        CHECK(pos == 200us);

        pos = pos1 + MicrosecondDuration{100us};
        CHECK(pos == 200us);

        pos = pos1 + FrameDuration{100us};
        CHECK(pos == 200us);
    }

    SECTION("subtraction")
    {
        const PlaybackPosition pos1{100us};
        const PlaybackPosition pos2{200us};

        PlaybackPosition pos;

        pos = pos2 - pos1;
        CHECK(pos == 100us);

        pos = PlaybackPosition{300us} - pos1;
        CHECK(pos == 200us);

        pos = PlaybackPosition{300us} - MicrosecondDuration{100us};
        CHECK(pos == 200us);

        pos = PlaybackPosition{300us} - FrameDuration{100us};
        CHECK(pos == 200us);
    }

    SECTION("min/max")
    {
        const PlaybackPosition pos{500us};

        const PlaybackPosition pos1 = std::max(pos - 2s, PlaybackPosition{0s});
        const PlaybackPosition pos2 = std::min(pos + 2s, PlaybackPosition{2s});

        CHECK(pos1 == 0s);
        CHECK(pos2 == 2s);
    }
}

}  // namespace video_trimmer::clock

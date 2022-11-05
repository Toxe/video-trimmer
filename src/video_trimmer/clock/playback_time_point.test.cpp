#include "catch2/catch_test_macros.hpp"

#include "playback_position.hpp"
#include "playback_time_point.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::clock {

TEST_CASE("clock::PlaybackTimePoint")
{
    SECTION("construction")
    {
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> tp_microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> tp_nanoseconds = std::chrono::steady_clock::now();

        const PlaybackTimePoint ptp_us{tp_microseconds};
        const PlaybackTimePoint ptp_ns{tp_nanoseconds};

        CHECK(ptp_us.microseconds() == tp_microseconds.time_since_epoch());
        CHECK(ptp_ns.microseconds() == std::chrono::time_point_cast<std::chrono::microseconds>(tp_nanoseconds).time_since_epoch());
    }

    SECTION("addition")
    {
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

        const PlaybackTimePoint ptp1{now};
        const PlaybackTimePoint ptp2 = ptp1 + 100us;

        CHECK(ptp2.microseconds() == now.time_since_epoch() + 100us);
    }

    SECTION("subtraction")
    {
        const std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> now = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::steady_clock::now());

        const PlaybackTimePoint ptp1{now};
        const PlaybackTimePoint ptp2{now + 100us};
        const PlaybackTimePoint ptp3 = ptp1 - 100us;

        CHECK(ptp3.microseconds() == now.time_since_epoch() - 100us);

        const PlaybackPosition pos = ptp2 - ptp1;

        CHECK(pos == 100us);
    }
}

}  // namespace video_trimmer::clock

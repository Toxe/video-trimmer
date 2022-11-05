#include "catch2/catch_test_macros.hpp"

#include "frame_duration.hpp"

using namespace std::literals::chrono_literals;

namespace video_trimmer::clock {

TEST_CASE("clock::FrameDuration")
{
    SECTION("construction")
    {
        const FrameDuration pos1{};
        const FrameDuration pos2{10us};
        const FrameDuration pos3 = FrameDuration{20us};

        CHECK(pos1.microseconds() == 0us);
        CHECK(pos2.microseconds() == 10us);
        CHECK(pos3.microseconds() == 20us);
    }

    SECTION("assignment")
    {
        FrameDuration pos1;

        pos1 = FrameDuration{10us};

        CHECK(pos1.microseconds() == 10us);
    }

    SECTION("comparison")
    {
        const FrameDuration pos1{100us};
        const FrameDuration pos2{200us};

        CHECK(pos1 < pos2);
        CHECK(pos2 > pos1);

        CHECK((pos1 == pos2) == false);
        CHECK(pos1 == FrameDuration{100us});

        CHECK(pos1 == 100us);
    }
}

}  // namespace video_trimmer::clock

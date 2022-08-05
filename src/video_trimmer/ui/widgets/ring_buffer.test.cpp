#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_floating_point.hpp"

#include "ring_buffer.hpp"

namespace video_trimmer::ui::widgets {

TEST_CASE("ui::widgets::RingBuffer")
{
    SECTION("Can create RingBuffers with different sizes")
    {
        RingBuffer buffer1(1);
        REQUIRE(buffer1.size() == 1);

        RingBuffer buffer2(10);
        REQUIRE(buffer2.size() == 10);

        RingBuffer buffer3(100);
        REQUIRE(buffer3.size() == 100);
    }

    SECTION("New RingBuffers need to have a size of at least 1")
    {
        REQUIRE_THROWS(RingBuffer(0));
        REQUIRE_THROWS(RingBuffer(-1));
    }

    SECTION("Can push and retrieve values")
    {
        RingBuffer buffer(10);

        buffer.push(1.0f);
        REQUIRE(buffer.get(0) == 1.0f);

        buffer.push(2.0f);
        REQUIRE(buffer.get(1) == 2.0f);

        buffer.push(3.0f);
        REQUIRE(buffer.get(2) == 3.0f);
    }

    SECTION("All values in a new RingBuffer are 0.0 by default")
    {
        RingBuffer buffer(5);

        for (int i = 0; i < buffer.size(); ++i)
            REQUIRE(buffer.get(i) == 0.0f);
    }

    SECTION("The offset points to the next insertion position")
    {
        RingBuffer buffer(10);

        REQUIRE(buffer.offset() == 0);

        buffer.push(1.0f);
        REQUIRE(buffer.offset() == 1);

        buffer.push(1.0f);
        REQUIRE(buffer.offset() == 2);
    }

    SECTION("Once the offset reaches the end it wraps around to the first element")
    {
        RingBuffer buffer(3);

        buffer.push(1.0f);
        buffer.push(2.0f);
        REQUIRE(buffer.offset() == 2);
        buffer.push(3.0f);
        REQUIRE(buffer.offset() == 0);
        buffer.push(4.0f);
        REQUIRE(buffer.offset() == 1);
    }

    SECTION("Can retrieve the last element")
    {
        RingBuffer buffer(3);

        for (int i = 0; i < 20; ++i) {
            const auto f = static_cast<float>(i);

            buffer.push(f);
            REQUIRE(buffer.last() == f);
        }
    }

    SECTION("Can retrieve a pointer to the underlying memory")
    {
        RingBuffer buffer(10);

        REQUIRE(buffer.data() != nullptr);
    }

    SECTION("max() returns the biggest element")
    {
        RingBuffer buffer(10);

        REQUIRE(buffer.max() == 0.0f);

        buffer.push(1.0f);
        REQUIRE(buffer.max() == 1.0f);

        buffer.push(3.0f);
        REQUIRE(buffer.max() == 3.0f);

        buffer.push(-1.0f);
        REQUIRE(buffer.max() == 3.0f);

        buffer.push(0.0f);
        REQUIRE(buffer.max() == 3.0f);

        buffer.push(5.0f);
        REQUIRE(buffer.max() == 5.0f);
    }
}

}  // namespace video_trimmer::ui::widgets

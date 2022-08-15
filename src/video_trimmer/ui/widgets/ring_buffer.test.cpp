#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "ring_buffer.hpp"

namespace video_trimmer::ui::widgets {

TEST_CASE("ui::widgets::RingBuffer")
{
    SECTION("Can create RingBuffers with different sizes")
    {
        RingBuffer buffer1(1);
        CHECK(buffer1.size() == 1);

        RingBuffer buffer2(10);
        CHECK(buffer2.size() == 10);

        RingBuffer buffer3(100);
        CHECK(buffer3.size() == 100);
    }

    SECTION("New RingBuffers need to have a size of at least 1")
    {
        CHECK_THROWS(RingBuffer(0));
        CHECK_THROWS(RingBuffer(-1));
    }

    SECTION("Can push and retrieve values")
    {
        RingBuffer buffer(10);

        buffer.push(1.0f);
        CHECK(buffer.get(0) == Catch::Approx(1.0f));

        buffer.push(2.0f);
        CHECK(buffer.get(1) == Catch::Approx(2.0f));

        buffer.push(3.0f);
        CHECK(buffer.get(2) == Catch::Approx(3.0f));
    }

    SECTION("All values in a new RingBuffer are 0.0 by default")
    {
        RingBuffer buffer(5);

        for (int i = 0; i < buffer.size(); ++i)
            CHECK(buffer.get(i) == Catch::Approx(0.0f));
    }

    SECTION("The offset points to the next insertion position")
    {
        RingBuffer buffer(10);

        CHECK(buffer.offset() == 0);

        buffer.push(1.0f);
        CHECK(buffer.offset() == 1);

        buffer.push(1.0f);
        CHECK(buffer.offset() == 2);
    }

    SECTION("Once the offset reaches the end it wraps around to the first element")
    {
        RingBuffer buffer(3);

        buffer.push(1.0f);
        buffer.push(2.0f);
        CHECK(buffer.offset() == 2);
        buffer.push(3.0f);
        CHECK(buffer.offset() == 0);
        buffer.push(4.0f);
        CHECK(buffer.offset() == 1);
    }

    SECTION("Can retrieve the last element")
    {
        RingBuffer buffer(3);

        for (int i = 0; i < 20; ++i) {
            const auto f = static_cast<float>(i);

            buffer.push(f);
            CHECK(buffer.last() == Catch::Approx(f));
        }
    }

    SECTION("Can retrieve a pointer to the underlying memory")
    {
        RingBuffer buffer(10);

        CHECK(buffer.data() != nullptr);
    }

    SECTION("max() returns the biggest element")
    {
        RingBuffer buffer(10);

        CHECK(buffer.max() == Catch::Approx(0.0f));

        buffer.push(1.0f);
        CHECK(buffer.max() == Catch::Approx(1.0f));

        buffer.push(3.0f);
        CHECK(buffer.max() == Catch::Approx(3.0f));

        buffer.push(-1.0f);
        CHECK(buffer.max() == Catch::Approx(3.0f));

        buffer.push(0.0f);
        CHECK(buffer.max() == Catch::Approx(3.0f));

        buffer.push(5.0f);
        CHECK(buffer.max() == Catch::Approx(5.0f));
    }
}

}  // namespace video_trimmer::ui::widgets

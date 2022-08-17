#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

#include "frame.hpp"

extern "C" {
#include "libavutil/frame.h"
}

namespace video_trimmer::video_file {

TEST_CASE("video_file::Frame")
{
    GIVEN("a default constructed audio frame")
    {
        const auto frame = Frame::create_audio_frame();

        WHEN("checking its timestamp")
        {
            THEN("returns 0.0")
            {
                CHECK(frame->timestamp() == Catch::Approx(0.0));
            }
        }

        WHEN("checking its duration")
        {
            THEN("returns 0.0")
            {
                CHECK(frame->duration() == Catch::Approx(0.0));
            }
        }

        WHEN("checking its size")
        {
            THEN("returns a size of 0/0")
            {
                CHECK(frame->size() == Size{0, 0});
            }
        }

        WHEN("checking its frame type")
        {
            THEN("it is an audio frame")
            {
                CHECK(frame->is_audio_frame() == true);
            }

            THEN("it is not a video frame")
            {
                CHECK(frame->is_video_frame() == false);
            }
        }

        WHEN("checking its PixelFormat")
        {
            THEN("has an invalid PixelFormat")
            {
                CHECK(!frame->pixel_format().is_valid());
            }
        }

        WHEN("requesting its description")
        {
            THEN("returns a correct description")
            {
                CHECK_THAT(frame->description(), Catch::Matchers::Equals("[Frame A @0.000 0.0000s]"));
            }
        }
    }

    GIVEN("a default constructed video frame")
    {
        const auto frame = Frame::create_video_frame();

        WHEN("checking its timestamp")
        {
            THEN("returns 0.0")
            {
                CHECK(frame->timestamp() == Catch::Approx(0.0));
            }
        }

        WHEN("checking its duration")
        {
            THEN("returns 0.0")
            {
                CHECK(frame->duration() == Catch::Approx(0.0));
            }
        }

        WHEN("checking its size")
        {
            THEN("returns a size of 0/0")
            {
                CHECK(frame->size() == Size{0, 0});
            }
        }

        WHEN("checking its frame type")
        {
            THEN("it is a video frame")
            {
                CHECK(frame->is_video_frame() == true);
            }

            THEN("it is not an audio frame")
            {
                CHECK(frame->is_audio_frame() == false);
            }
        }

        WHEN("checking its PixelFormat")
        {
            THEN("has an invalid PixelFormat")
            {
                CHECK(!frame->pixel_format().is_valid());
            }
        }

        WHEN("requesting its description")
        {
            THEN("returns a correct description")
            {
                CHECK_THAT(frame->description(), Catch::Matchers::Equals("[Frame V? @0.000 0.0000s, 0x0, pts=0:0 (0), pkt_duration=0]"));
            }
        }
    }

    GIVEN("a video frame constructed with explicit values")
    {
        const auto frame = Frame::create_video_frame(Size{640, 480}, PixelFormat{1}, 1837735, 10.0, 1.0 / 60.0, 'I');

        WHEN("checking its timestamp")
        {
            THEN("returns correct timestamp")
            {
                CHECK(frame->timestamp() == Catch::Approx(10.0));
            }
        }

        WHEN("checking its duration")
        {
            THEN("returns correct duration")
            {
                CHECK(frame->duration() == Catch::Approx(1.0 / 60.0));
            }
        }

        WHEN("checking its size")
        {
            THEN("returns correct size")
            {
                CHECK(frame->size() == Size{640, 480});
            }
        }

        WHEN("checking its frame type")
        {
            THEN("it is a video frame")
            {
                CHECK(frame->is_video_frame() == true);
            }

            THEN("it is not an audio frame")
            {
                CHECK(frame->is_audio_frame() == false);
            }
        }

        WHEN("checking its PixelFormat")
        {
            THEN("has a valid PixelFormat")
            {
                CHECK(frame->pixel_format().is_valid());
            }
        }

        WHEN("requesting its description")
        {
            THEN("returns a correct description")
            {
                CHECK_THAT(frame->description(), Catch::Matchers::Equals("[Frame VI @10.000 0.0167s, 640x480, pts=0:1837735 (-1837735), pkt_duration=0]"));
            }
        }
    }

    GIVEN("a video frame")
    {
        WHEN("updating its values from an AVFrame")
        {
            THEN("it sets new values")
            {
                const auto frame = Frame::create_video_frame(Size{640, 480}, PixelFormat{1}, 1837735);

                frame->frame()->best_effort_timestamp = 4503;
                frame->frame()->pts = 4503;
                frame->frame()->pkt_duration = 1501;
                frame->frame()->pict_type = AV_PICTURE_TYPE_I;
                frame->update_from_frame(0.00001111111111111111);

                CHECK_THAT(frame->description(), Catch::Matchers::Equals("[Frame VI @0.050 0.0167s, 640x480, pts=4503:1837735 (-1833232), pkt_duration=1501]"));
            }
        }
    }
}

}  // namespace video_trimmer::video_file

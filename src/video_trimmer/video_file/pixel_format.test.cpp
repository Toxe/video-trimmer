#include "catch2/catch_test_macros.hpp"
#include "catch2/matchers/catch_matchers_string.hpp"

#include "pixel_format.hpp"

#include "SDL2/SDL_pixels.h"

extern "C" {
#include "libavutil/pixfmt.h"
}

namespace video_trimmer::video_file {

TEST_CASE("video_file::PixelFormat")
{
    GIVEN("a default constructed PixelFormat")
    {
        const PixelFormat pixel_format;

        WHEN("requesting its AVPixelFormat value")
        {
            THEN("it returns AV_PIX_FMT_NONE")
            {
                CHECK(pixel_format.av_pixel_format() == AV_PIX_FMT_NONE);
            }
        }

        WHEN("requesting its name")
        {
            THEN("it returns \"N/A\"")
            {
                CHECK_THAT(pixel_format.name(), Catch::Matchers::Equals("N/A"));
            }
        }

        WHEN("requesting a compatible SDL pixel format")
        {
            THEN("it returns SDL_PIXELFORMAT_UNKNOWN")
            {
                CHECK(pixel_format.get_compatible_sdl_pixel_format() == SDL_PIXELFORMAT_UNKNOWN);
            }
        }

        WHEN("checking if it is supported")
        {
            THEN("it returns false")
            {
                CHECK(pixel_format.is_supported() == false);
            }
        }

        WHEN("checking if it is a valid pixel format")
        {
            THEN("it returns false")
            {
                CHECK(pixel_format.is_valid() == false);
            }
        }
    }

    GIVEN("a PixelFormat representing AV_PIX_FMT_YUV420P")
    {
        const PixelFormat pixel_format{AV_PIX_FMT_YUV420P};

        WHEN("requesting its AVPixelFormat value")
        {
            THEN("it returns AV_PIX_FMT_YUV420P")
            {
                CHECK(pixel_format.av_pixel_format() == AV_PIX_FMT_YUV420P);
            }
        }

        WHEN("requesting its name")
        {
            THEN("it returns \"yuv420p\"")
            {
                CHECK_THAT(pixel_format.name(), Catch::Matchers::Equals("yuv420p"));
            }
        }

        WHEN("requesting a compatible SDL pixel format")
        {
            THEN("it returns SDL_PIXELFORMAT_IYUV")
            {
                CHECK(pixel_format.get_compatible_sdl_pixel_format() == SDL_PIXELFORMAT_IYUV);
            }
        }

        WHEN("checking if it is supported")
        {
            THEN("it returns true")
            {
                CHECK(pixel_format.is_supported() == true);
            }
        }

        WHEN("checking if it is a valid pixel format")
        {
            THEN("it returns true")
            {
                CHECK(pixel_format.is_valid() == true);
            }
        }
    }

    GIVEN("two PixelFormat objects with the same AVPixelFormat values")
    {
        WHEN("comparing the two objects")
        {
            THEN("they are equal")
            {
                CHECK(PixelFormat() == PixelFormat());
                CHECK(PixelFormat(AV_PIX_FMT_YUV420P) == PixelFormat(AV_PIX_FMT_YUV420P));
                CHECK(PixelFormat(AV_PIX_FMT_RGB24) == PixelFormat(AV_PIX_FMT_RGB24));
            }
        }
    }

    GIVEN("two PixelFormat objects with different AVPixelFormat values")
    {
        WHEN("comparing the two objects")
        {
            THEN("they are not equal")
            {
                CHECK(PixelFormat() != PixelFormat(AV_PIX_FMT_YUV420P));
                CHECK(PixelFormat(AV_PIX_FMT_YUV420P) != PixelFormat());
                CHECK(PixelFormat(AV_PIX_FMT_YUV420P) != PixelFormat(AV_PIX_FMT_RGB24));
            }
        }
    }
}

}  // namespace video_trimmer::video_file

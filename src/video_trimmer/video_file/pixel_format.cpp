#include "pixel_format.hpp"

#include "SDL2/SDL_pixels.h"

extern "C" {
#include "libavutil/pixdesc.h"
#include "libavutil/pixfmt.h"
}

namespace video_trimmer::video_file {

PixelFormat::PixelFormat()
    : av_pixel_format_(AV_PIX_FMT_NONE)
{
}

PixelFormat::PixelFormat(int av_pixel_format)
    : av_pixel_format_(av_pixel_format)
{
}

const char* PixelFormat::name() const
{
    if (av_pixel_format_ == AV_PIX_FMT_NONE)
        return "N/A";

    return av_get_pix_fmt_name(static_cast<AVPixelFormat>(av_pixel_format_));
}

bool PixelFormat::is_supported() const
{
    return av_pixel_format_ == AV_PIX_FMT_YUV420P;
}

bool PixelFormat::is_valid() const
{
    return av_pixel_format_ != AV_PIX_FMT_NONE;
}

int PixelFormat::get_compatible_sdl_pixel_format() const
{
    switch (av_pixel_format_) {
        case AV_PIX_FMT_YUV420P: return SDL_PIXELFORMAT_IYUV;
        case AV_PIX_FMT_YUYV422: return SDL_PIXELFORMAT_YUY2;
        case AV_PIX_FMT_UYVY422: return SDL_PIXELFORMAT_UYVY;
        default:
            return SDL_PIXELFORMAT_UNKNOWN;
    }
}

}  // namespace video_trimmer::video_file

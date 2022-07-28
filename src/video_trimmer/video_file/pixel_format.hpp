#pragma once

namespace video_trimmer::video_file {

class PixelFormat {
public:
    PixelFormat();
    explicit PixelFormat(int av_pixel_format);

    [[nodiscard]] int av_pixel_format() const { return av_pixel_format_; }
    [[nodiscard]] int get_compatible_sdl_pixel_format() const;

    [[nodiscard]] const char* name() const;
    [[nodiscard]] bool is_supported() const;

private:
    int av_pixel_format_;
};

inline bool operator==(const PixelFormat& lhs, const PixelFormat& rhs) { return lhs.av_pixel_format() == rhs.av_pixel_format(); }

}  // namespace video_trimmer::video_file

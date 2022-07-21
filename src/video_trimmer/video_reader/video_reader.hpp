#pragma once

#include <memory>

#include "frame/frame.hpp"
#include "video_file/video_file.hpp"

namespace video_trimmer::video_reader {

class VideoReader {
public:
    VideoReader(video_file::VideoFile& video_file);

    [[nodiscard]] std::unique_ptr<frame::Frame> read_next_frame(double playback_position);

private:
    video_file::VideoFile& video_file_;
};

}  // namespace video_trimmer::video_reader

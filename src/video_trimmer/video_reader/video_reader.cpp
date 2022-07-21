#include "video_reader.hpp"

#include "fmt/core.h"

#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::video_reader {

VideoReader::VideoReader(video_file::VideoFile& video_file)
    : video_file_(video_file)
{
}

std::unique_ptr<frame::Frame> VideoReader::read_next_frame(const double playback_position)
{
    std::unique_ptr<frame::Frame> frame = video_file_.read_next_frame(playback_position);

    return frame;
}

}  // namespace video_trimmer::video_reader

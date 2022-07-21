#include "video_reader.hpp"

namespace video_trimmer::video_reader {

VideoReader::VideoReader(video_file::VideoFile& video_file)
    : video_file_(video_file)
{
}

std::unique_ptr<frame::Frame> VideoReader::read_next_frame(const double playback_position)
{
    return video_file_.read_next_frame(playback_position);
}

}  // namespace video_trimmer::video_reader

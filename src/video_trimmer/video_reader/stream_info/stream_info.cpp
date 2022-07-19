#include "stream_info.hpp"

namespace video_trimmer::video_reader::stream_info {

StreamInfo::StreamInfo(std::unique_ptr<codec_context::CodecContext> codec_context, int stream_index)
    : codec_context_{std::move(codec_context)}, stream_index_{stream_index}
{
}

}  // namespace video_trimmer::video_reader::stream_info

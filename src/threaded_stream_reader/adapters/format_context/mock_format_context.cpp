#include "mock_format_context.hpp"

#include "../../factory/factory.hpp"
#include "../codec_context/codec_context.hpp"
#include "../packet/packet.hpp"
#include "error/error.hpp"

constexpr double video_time_base = 1.0 / 60.0;
constexpr double audio_time_base = 1.0 / 48000.0;

double MockFormatContext::stream_time_base(const int stream_index) const
{
    return stream_index == 0 ? video_time_base : audio_time_base;
}

const std::string MockFormatContext::format() const
{
    return "";
}

std::unique_ptr<StreamInfo> MockFormatContext::find_best_stream(Factory* factory, const StreamType type)
{
    const int stream_index = type == StreamType::video ? 0 : 1;

    std::unique_ptr<CodecContext> codec_context = factory->create_codec_context(nullptr);

    return std::make_unique<StreamInfo>(this, std::move(codec_context), stream_index);
}

int MockFormatContext::read_frame(Packet* packet)
{
    if (num_packets_[0] >= max_number_of_video_frames_to_return_)
        return -1;  // end of file

    ++num_packets_[static_cast<std::size_t>(read_stream_index_)];
    packet->set_stream_index(read_stream_index_);
    read_stream_index_ = read_stream_index_ == 0 ? 1 : 0;

    return 0;
}

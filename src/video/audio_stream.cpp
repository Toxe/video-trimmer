#include "audio_stream.hpp"

#include <spdlog/spdlog.h>

#include "error.hpp"

AudioStream::AudioStream(AVFormatContext* format_context, AVCodecContext* codec_context, int stream_index)
    : format_context_{format_context}, codec_context_{codec_context}, stream_index_{stream_index}
{
    spdlog::trace("init AudioStream");

    is_ready_ = init_stream() == 0;
}

int AudioStream::init_stream()
{
    return 0;
}

bool AudioStream::next_frame()
{
    return true;
}

int AudioStream::decode_packet(const AVPacket* packet)
{
    return 0;
}

#pragma once

extern "C" {
#include <libavcodec/packet.h>
#include <libavformat/avformat.h>
}

#include "auto_delete_ressource.hpp"
#include "types.h"

class AudioStream;
class VideoStream;

class VideoContentProvider {
    AVFormatContext* format_context_ = nullptr;

    VideoStream& video_stream_;
    AudioStream& audio_stream_;

    auto_delete_ressource<AVPacket> packet_ = {nullptr, nullptr};

    bool is_ready_ = false;

    int init();

public:
    VideoContentProvider(AVFormatContext* format_context, VideoStream& video_stream, AudioStream& audio_stream);

    bool read(ImageSize video_size);

    const uint8_t* next_video_frame();
};

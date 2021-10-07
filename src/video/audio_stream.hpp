#pragma once

#include <array>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "auto_delete_ressource.hpp"

class AudioStream {
    AVFormatContext* format_context_ = nullptr;
    AVCodecContext* codec_context_ = nullptr;

    int stream_index_ = -1;

    bool is_ready_ = false;

    [[nodiscard]] int init_stream();

public:
    AudioStream(AVFormatContext* format_context, AVCodecContext* codec_context, int stream_index);

    [[nodiscard]] bool is_ready() const { return is_ready_; }
    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] int decode_packet(const AVPacket* packet);

    bool next_frame();
};

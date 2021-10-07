#pragma once

#include <array>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "auto_delete_ressource.hpp"
#include "types.h"

class VideoStream {
    AVFormatContext* format_context_ = nullptr;
    AVCodecContext* codec_context_ = nullptr;

    auto_delete_ressource<AVFrame> frame_ = {nullptr, nullptr};
    auto_delete_ressource<SwsContext> scaling_context_ = {nullptr, nullptr};

    int stream_index_ = -1;

    std::array<uint8_t*, 4> img_buf_data_ = {nullptr};
    std::array<uint8_t*, 4> dst_buf_data_ = {nullptr};
    std::array<int, 4> img_buf_linesize_ = {0};
    std::array<int, 4> dst_buf_linesize_ = {0};
    int img_buf_size_ = 0;
    int dst_buf_size_ = 0;

    int scale_width_ = 0;
    int scale_height_ = 0;

    bool is_ready_ = false;
    bool has_frame_ = false;

    [[nodiscard]] int init_stream();

    int resize_scaling_context(AVCodecContext* codec_context, int width, int height);

public:
    VideoStream(AVFormatContext* format_context, AVCodecContext* codec_context, int stream_index);

    [[nodiscard]] bool is_ready() const { return is_ready_; }
    [[nodiscard]] int stream_index() const { return stream_index_; }

    [[nodiscard]] int decode_packet(const AVPacket* packet, ImageSize video_size);

    const uint8_t* next_frame();
};

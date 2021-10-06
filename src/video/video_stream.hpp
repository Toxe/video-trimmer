#pragma once

#include <array>
#include <optional>
#include <string_view>

#include <SFML/Graphics/Texture.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

#include "auto_delete_ressource.hpp"

class VideoStream {
    AVFormatContext* format_context_ = nullptr;
    AVCodecContext* video_codec_context_ = nullptr;

    auto_delete_ressource<AVFrame> frame_ = {nullptr, nullptr};
    auto_delete_ressource<AVPacket> packet_ = {nullptr, nullptr};
    auto_delete_ressource<SwsContext> scaling_context_ = {nullptr, nullptr};

    int video_stream_index_ = -1;

    std::array<uint8_t*, 4> img_buf_data_ = {nullptr};
    std::array<uint8_t*, 4> dst_buf_data_ = {nullptr};
    std::array<int, 4> img_buf_linesize_ = {0};
    std::array<int, 4> dst_buf_linesize_ = {0};
    int img_buf_size_ = 0;
    int dst_buf_size_ = 0;

    int scale_width_ = 0;
    int scale_height_ = 0;

    bool is_ready_ = false;

    int show_error(const std::string_view& error_message, std::optional<int> error_code = std::nullopt);
    int init_stream();

    int resize_scaling_context(AVCodecContext* codec_context, int width, int height);

    [[nodiscard]] int decode_packet(AVCodecContext* codec_context, const AVPacket* packet, AVFrame* frame, sf::Texture& texture);

public:
    VideoStream(AVFormatContext* format_context, AVCodecContext* video_codec_context, int video_stream_index);

    bool is_ready() const { return is_ready_; }

    bool next_frame(sf::Texture& texture);
};

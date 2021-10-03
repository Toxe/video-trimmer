#include "video_file.hpp"

#include <cstdlib>
#include <filesystem>
#include <memory>

#include <fmt/core.h>
#include <spdlog/spdlog.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

bool error(const std::string& error_message)
{
    spdlog::error(error_message);
    return false;
}

VideoFile::VideoFile(const std::string& full_filename)
{
    is_open_ = open_file(full_filename);
}

bool VideoFile::open_file(const std::string& full_filename)
{
    spdlog::info("open video: {}", full_filename);

    std::filesystem::path fp{full_filename};

    if (!std::filesystem::exists(fp))
        return error("file not found");

    filename_ = fp.filename().string();

    std::unique_ptr<AVFormatContext, void (*)(AVFormatContext*)> format_context(avformat_alloc_context(), [](AVFormatContext* fmt_ctx) { avformat_close_input(&fmt_ctx); });

    if (!format_context)
        return error("avformat_alloc_context");

    auto p = format_context.get();

    if (avformat_open_input(&p, full_filename.c_str(), nullptr, nullptr) < 0)
        return error("avformat_open_input");

    file_format_ = format_context->iformat->long_name;

    if (avformat_find_stream_info(format_context.get(), nullptr) < 0)
        return error("avformat_find_stream_info");

    for (unsigned int i = 0; i < format_context->nb_streams; ++i) {
        auto codec_params = format_context->streams[i]->codecpar;
        auto codec = avcodec_find_decoder(codec_params->codec_id);

        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO) {
            streams_.push_back(StreamInfo{av_get_media_type_string(codec_params->codec_type), codec->long_name, fmt::format("{}x{}", codec_params->width, codec_params->height)});
        } else if (codec_params->codec_type == AVMEDIA_TYPE_AUDIO) {
            streams_.push_back(StreamInfo{av_get_media_type_string(codec_params->codec_type), codec->long_name, fmt::format("{} channels, {} sample rate", codec_params->channels, codec_params->sample_rate)});
        }
    }

    return true;
}

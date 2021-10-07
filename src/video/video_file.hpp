#pragma once

#include <string>
#include <tuple>
#include <vector>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

#include "auto_delete_ressource.hpp"
#include "stream_info.hpp"
#include "video_stream.hpp"

class VideoFile {
    auto_delete_ressource<AVFormatContext> format_context_ = {nullptr, nullptr};
    auto_delete_ressource<AVCodecContext> audio_codec_context_ = {nullptr, nullptr};
    auto_delete_ressource<AVCodecContext> video_codec_context_ = {nullptr, nullptr};

    int audio_stream_index_ = -1;
    int video_stream_index_ = -1;

    bool is_open_ = false;

    std::string filename_without_path_;
    std::string file_format_;
    std::vector<StreamInfo> streams_;

    [[nodiscard]] std::tuple<int, auto_delete_ressource<AVCodecContext>> find_best_stream(AVFormatContext* format_context, const AVMediaType type);

    int open_file(const std::string& full_filename);

public:
    VideoFile(const std::string& full_filename);

    [[nodiscard]] bool is_open() const { return is_open_; }
    [[nodiscard]] const std::string& filename() const { return filename_without_path_; }
    [[nodiscard]] const std::string& file_format() const { return file_format_; }
    [[nodiscard]] int number_of_streams() const { return static_cast<int>(streams_.size()); }
    [[nodiscard]] const StreamInfo& stream_info(const int stream_id) const { return streams_[stream_id]; }

    [[nodiscard]] VideoStream open_video_stream() const;
};

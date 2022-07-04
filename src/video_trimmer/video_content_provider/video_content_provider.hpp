#pragma once

#include <memory>

#include "video_file/video_file.hpp"
#include "video_frame_scaler/video_frame_scaler.hpp"
#include "video_reader/video_reader.hpp"

class VideoContentProvider {
public:
    VideoContentProvider(VideoFile& video_file, int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<VideoFrame> next_frame(double playback_position);

    void change_scaling_dimensions(int scale_width, int scale_height);

private:
    VideoFrameScaler video_frame_scaler_;
    VideoReader video_reader_;
};

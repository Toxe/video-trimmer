#pragma once

#include <memory>

#include "video_file/video_file.hpp"
#include "video_frame_scaler/video_frame_scaler.hpp"
#include "video_reader/video_reader.hpp"

namespace video_trimmer::video_content_provider {

class VideoContentProvider {
public:
    VideoContentProvider(video_file::VideoFile& video_file, int scale_width, int scale_height);

    [[nodiscard]] std::unique_ptr<video_frame::VideoFrame> next_frame(double playback_position);

    void change_scaling_dimensions(int scale_width, int scale_height);

private:
    video_frame_scaler::VideoFrameScaler video_frame_scaler_;
    video_reader::VideoReader video_reader_;
};

}  // namespace video_trimmer::video_content_provider

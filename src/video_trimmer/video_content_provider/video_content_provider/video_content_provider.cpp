#include "video_content_provider.hpp"

VideoContentProvider::VideoContentProvider(VideoFile& video_file, int scale_width, int scale_height)
    : video_frame_scaler_{video_file.video_stream_info(), scale_width, scale_height},
      video_reader_{video_file.audio_stream_info(), video_file.video_stream_info(), scale_width, scale_height}
{
}

std::unique_ptr<VideoFrame> VideoContentProvider::next_frame(const double playback_position)
{
    auto video_frame = video_reader_.read();

    if (video_frame)
        video_frame_scaler_.scale_frame(video_frame.get());

    return video_frame;
}

void VideoContentProvider::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    video_reader_.change_scaling_dimensions(scale_width, scale_height);
}

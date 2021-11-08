#include "video_content_provider.hpp"

#include <latch>

#include <fmt/core.h>

#include "../factory/factory.hpp"
#include "../video_frame/video_frame.hpp"
#include "error/error.hpp"
#include "logger/logger.hpp"

VideoContentProvider::VideoContentProvider(Factory* factory, VideoFile& video_file, const int scale_width, const int scale_height)
    : factory_{factory},
      video_frame_scaler_{factory, video_file.video_stream_info(), scale_width, scale_height},
      video_reader_{factory, video_file.audio_stream_info(), video_file.video_stream_info(), scale_width, scale_height}
{
}

VideoContentProvider::~VideoContentProvider()
{
    stop();
}

void VideoContentProvider::run()
{
    if (!is_running_) {
        log_debug("(VideoContentProvider) run");

        std::latch latch{3};

        video_frame_scaler_.run(this, latch);
        video_reader_.run(this, latch);

        latch.arrive_and_wait();

        is_running_ = true;
    }
}

void VideoContentProvider::stop()
{
    if (is_running_) {
        log_debug("(VideoContentProvider) stop");

        video_reader_.stop();
        video_frame_scaler_.stop();

        is_running_ = false;
    }
}

bool VideoContentProvider::has_finished()
{
    return video_reader_.has_finished() && video_frame_scaler_.has_finished();
}

bool VideoContentProvider::video_reader_has_finished()
{
    return video_reader_.has_finished();
}

bool VideoContentProvider::finished_video_frames_queue_is_not_full()
{
    std::lock_guard<std::mutex> lock(mtx_);
    return !finished_video_frames_queue_.full();
}

void VideoContentProvider::add_video_frame_for_scaling(std::unique_ptr<VideoFrame> video_frame)
{
    video_frame_scaler_.add_to_queue(std::move(video_frame));
}

void VideoContentProvider::add_finished_video_frame(std::unique_ptr<VideoFrame> video_frame)
{
    std::lock_guard<std::mutex> lock(mtx_);
    finished_video_frames_queue_.push(std::move(video_frame));
}

std::tuple<std::unique_ptr<VideoFrame>, int> VideoContentProvider::next_frame(const double playback_position)
{
    std::lock_guard<std::mutex> lock(mtx_);

    std::unique_ptr<VideoFrame> video_frame = finished_video_frames_queue_.pop(playback_position);

    if (video_frame && !finished_video_frames_queue_.full())
        video_reader_.wakeup();

    if (!video_frame && video_reader_has_finished())
        video_frame_scaler_.wakeup();  // work done, wake up scaler so that it can quit

    return std::make_tuple(std::move(video_frame), finished_video_frames_queue_.size());
}

void VideoContentProvider::change_scaling_dimensions(const int scale_width, const int scale_height)
{
    video_reader_.change_scaling_dimensions(scale_width, scale_height);
}

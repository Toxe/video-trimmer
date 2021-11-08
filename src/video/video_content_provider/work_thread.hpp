#pragma once

#include <atomic>
#include <condition_variable>
#include <latch>
#include <mutex>
#include <stop_token>
#include <thread>

#include "run_state.hpp"
#include "work_thread.hpp"

class Factory;
class Packet;
class StreamInfo;
class VideoContentProvider;
class VideoFrame;

class WorkThread {
    Factory* factory_;

    std::jthread thread_;
    std::atomic<RunState> state_ = RunState::starting;

    const char* log_class_name_;

    virtual void main(std::stop_token st, VideoContentProvider* video_content_provider, std::latch& latch) = 0;

protected:
    std::mutex mtx_;
    std::condition_variable_any cv_;

    Factory* factory() const { return factory_; }

public:
    WorkThread(Factory* factory, const char* log_class_name);
    virtual ~WorkThread();

    void run(VideoContentProvider* video_content_provider, std::latch& latch);
    void stop();
    void wakeup();

    void set_state(RunState state);

    [[nodiscard]] bool has_finished();
};

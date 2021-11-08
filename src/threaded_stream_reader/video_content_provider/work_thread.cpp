#include "work_thread.hpp"

#include <fmt/core.h>

#include "logger/logger.hpp"

WorkThread::WorkThread(Factory* factory, const char* log_class_name)
    : factory_{factory}, log_class_name_{log_class_name}
{
}

WorkThread::~WorkThread()
{
    stop();
}

void WorkThread::run(VideoContentProvider* video_content_provider, std::latch& latch)
{
    if (!thread_.joinable()) {
        log_debug(fmt::format("({}) run", log_class_name_));

        thread_ = std::jthread([this, video_content_provider, &latch](std::stop_token st) { main(std::move(st), video_content_provider, latch); });
    }
}

void WorkThread::stop()
{
    if (thread_.joinable()) {
        log_debug(fmt::format("({}) stop", log_class_name_));

        thread_.request_stop();
        thread_.join();
    }
}

void WorkThread::wakeup()
{
    cv_.notify_one();
}

void WorkThread::set_state(RunState state)
{
    state_ = state;
}

bool WorkThread::has_finished()
{
    return state_ == RunState::fnished;
}

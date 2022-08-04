#include "app.hpp"

#include "system_info.hpp"

namespace video_trimmer::app {

App::App()
{
    show_system_info();
}

void App::begin_frame()
{
    frame_time_clock_.restart();
}

void App::set_previous_frame_time(clock::Duration frame_time)
{
    previous_frame_time_ = frame_time;
}

}  // namespace video_trimmer::app

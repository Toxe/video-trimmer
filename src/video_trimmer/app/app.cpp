#include "app.hpp"

#include "system_info.hpp"

namespace video_trimmer::app {

App::App()
{
    show_system_info();
}

void App::begin_frame()
{
    elapsed_time_ = frame_time_clock_.restart();
}

}  // namespace video_trimmer::app

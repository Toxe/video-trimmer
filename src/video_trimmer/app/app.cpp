#include "app.hpp"

namespace video_trimmer::app {

void App::next_frame()
{
    elapsed_time_ = frame_time_clock_.restart();
}

}  // namespace video_trimmer::app

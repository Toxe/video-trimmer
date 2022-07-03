#include "app.h"

void video_trimmer::App::next_frame()
{
    elapsed_time_ = frame_time_clock_.restart();
}

#include "app.h"

void App::next_frame()
{
    elapsed_time_ = frame_time_clock_.restart();
}

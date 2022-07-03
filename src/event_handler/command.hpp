#pragma once

#include <functional>

namespace video_trimmer::event_handler {

using Command = std::function<void()>;

}  // namespace video_trimmer::event_handler

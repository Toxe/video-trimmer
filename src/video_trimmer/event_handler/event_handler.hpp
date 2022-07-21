#pragma once

#include <memory>

#include "command.hpp"
#include "events.hpp"

namespace video_trimmer::event_handler {

class EventHandler {
public:
    EventHandler();
    ~EventHandler();

    void set_command(const Event& event, Command command);

    void poll_events();
    void handle_event(const Event& event);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::event_handler

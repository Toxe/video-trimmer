#pragma once

#include <unordered_map>

#include "command.hpp"
#include "events.hpp"

namespace sf {
class RenderWindow;
}

namespace video_trimmer::event_handler {

class EventHandler {
public:
    EventHandler();

    void set_command(const Event& key_event, Command command);

    void poll_events(sf::RenderWindow& window);
    void handle_event(const Event& event);

private:
    std::unordered_map<Event, Command> commands_;
};

}  // namespace video_trimmer::event_handler

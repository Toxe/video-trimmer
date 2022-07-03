#pragma once

#include <unordered_map>

#include "command.hpp"
#include "events.hpp"

namespace sf {
class RenderWindow;
}

namespace video_trimmer::event_handler {

class EventHandler {
    std::unordered_map<Event, Command> commands_;

public:
    EventHandler();

    void handle_event(const Event& event);

    void set_command(const Event& key_event, Command command);

    void poll_events(sf::RenderWindow& window);
};

}  // namespace video_trimmer::event_handler

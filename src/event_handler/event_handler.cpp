#include "event_handler.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "events.hpp"
#include "logger/logger.hpp"

const video_trimmer::event_handler::Command no_command = [] { log_debug("NoCommand"); };

video_trimmer::event_handler::EventHandler::EventHandler()
{
    commands_[Event::CloseWindow] = no_command;
    commands_[Event::ResizedWindow] = no_command;
    commands_[Event::ToggleHelp] = no_command;
}

void video_trimmer::event_handler::EventHandler::set_command(const Event& event, Command command)
{
    commands_[event] = command;
}

void video_trimmer::event_handler::EventHandler::handle_event(const Event& event)
{
    commands_[event]();
}

void video_trimmer::event_handler::EventHandler::poll_events(sf::RenderWindow& window)
{
    sf::Event event;

    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);

        if (event.type == sf::Event::Closed)
            handle_event(Event::CloseWindow);
        else if (event.type == sf::Event::Resized)
            handle_event(Event::ResizedWindow);
        else if (event.type == sf::Event::KeyPressed) {
            if (ImGui::GetIO().WantCaptureKeyboard)
                continue;

            if (event.key.code == sf::Keyboard::Escape)
                handle_event(Event::CloseWindow);
            else if (event.key.code == sf::Keyboard::F1)
                handle_event(Event::ToggleHelp);
        }
    }
}

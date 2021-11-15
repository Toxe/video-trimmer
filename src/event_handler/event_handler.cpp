#include "event_handler.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include "logger/logger.hpp"

const Command no_command = [] { log_debug("NoCommand"); };

EventHandler::EventHandler()
{
    commands_[Event::CloseWindow]   = no_command;
    commands_[Event::ResizedWindow] = no_command;
    commands_[Event::ToggleHelp]    = no_command;
}

void EventHandler::set_command(const Event& event, Command command)
{
    commands_[event] = command;
}

void EventHandler::handle_event(const Event& event)
{
    commands_[event]();
}

void EventHandler::poll_events(sf::RenderWindow& window)
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

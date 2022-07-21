#include "event_handler.hpp"

#include "SDL2/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "events.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::event_handler {

const Command no_command = [] { video_trimmer::logger::log_debug("NoCommand"); };

void handle_window_events(EventHandler& event_handler, const SDL_Event& event)
{
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        event_handler.handle_event(Event::ResizedWindow);
    else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        event_handler.handle_event(Event::CloseWindow);
}

void handle_keyboard_events(EventHandler& event_handler, const SDL_Event& event)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (event.key.keysym.sym == SDLK_ESCAPE)
        event_handler.handle_event(Event::CloseWindow);
    else if (event.key.keysym.sym == SDLK_F1)
        event_handler.handle_event(Event::ToggleHelp);
    else if (event.key.keysym.sym == SDLK_SPACE)
        event_handler.handle_event(Event::PlaybackTogglePause);
}

EventHandler::EventHandler()
{
    commands_[Event::CloseWindow] = no_command;
    commands_[Event::ResizedWindow] = no_command;
    commands_[Event::ToggleHelp] = no_command;
}

void EventHandler::set_command(const Event& event, Command command)
{
    commands_[event] = command;
}

void EventHandler::handle_event(const Event& event)
{
    commands_[event]();
}

void EventHandler::poll_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            handle_event(Event::CloseWindow);
        else if (event.type == SDL_WINDOWEVENT)
            handle_window_events(*this, event);
        else if (event.type == SDL_KEYDOWN)
            handle_keyboard_events(*this, event);
    }
}

}  // namespace video_trimmer::event_handler

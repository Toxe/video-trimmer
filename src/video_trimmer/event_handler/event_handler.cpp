#include "event_handler.hpp"

#include "SDL2/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "events.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::event_handler {

const Command no_command = [] { video_trimmer::logger::log_debug("NoCommand"); };

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
        else if (event.type == SDL_WINDOWEVENT) {
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                handle_event(Event::ResizedWindow);
            else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                handle_event(Event::CloseWindow);
        } else if (event.type == SDL_KEYDOWN) {
            if (ImGui::GetIO().WantCaptureKeyboard)
                continue;

            if (event.key.keysym.sym == SDLK_ESCAPE)
                handle_event(Event::CloseWindow);
            else if (event.key.keysym.sym == SDLK_F1)
                handle_event(Event::ToggleHelp);
        }
    }
}

}  // namespace video_trimmer::event_handler

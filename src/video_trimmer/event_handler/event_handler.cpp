#include "event_handler.hpp"

#include <unordered_map>

#include "SDL2/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "events.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::event_handler {

const Command no_command = [] { video_trimmer::logger::log_debug("NoCommand"); };

class EventHandler::Impl {
public:
    Impl();

    void set_command(const Event& event, Command command);

    void poll_events();
    void handle_event(const Event& event);

private:
    std::unordered_map<Event, Command> commands_;

    void handle_window_events(const SDL_Event& event);
    void handle_keyboard_events(const SDL_Event& event);
};

EventHandler::Impl::Impl()
{
    commands_[Event::CloseWindow] = no_command;
    commands_[Event::ResizedWindow] = no_command;
    commands_[Event::ToggleHelp] = no_command;
    commands_[Event::PlaybackStart] = no_command;
    commands_[Event::PlaybackTogglePause] = no_command;
}

void EventHandler::Impl::set_command(const Event& event, Command command)
{
    commands_[event] = command;
}

void EventHandler::Impl::handle_event(const Event& event)
{
    commands_[event]();
}

void EventHandler::Impl::poll_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if (event.type == SDL_QUIT)
            handle_event(Event::CloseWindow);
        else if (event.type == SDL_WINDOWEVENT)
            handle_window_events(event);
        else if (event.type == SDL_KEYDOWN)
            handle_keyboard_events(event);
    }
}

void EventHandler::Impl::handle_window_events(const SDL_Event& event)
{
    if (event.window.event == SDL_WINDOWEVENT_RESIZED)
        handle_event(Event::ResizedWindow);
    else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        handle_event(Event::CloseWindow);
}

void EventHandler::Impl::handle_keyboard_events(const SDL_Event& event)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    if (event.key.keysym.sym == SDLK_ESCAPE)
        handle_event(Event::CloseWindow);
    else if (event.key.keysym.sym == SDLK_F1)
        handle_event(Event::ToggleHelp);
    else if (event.key.keysym.sym == SDLK_SPACE)
        handle_event(Event::PlaybackTogglePause);
}

EventHandler::EventHandler() : impl_(std::make_unique<EventHandler::Impl>()) { }
EventHandler::~EventHandler() = default;
void EventHandler::set_command(const Event& event, Command command) { impl_->set_command(event, command); }
void EventHandler::poll_events() { impl_->poll_events(); }
void EventHandler::handle_event(const Event& event) { impl_->handle_event(event); }

}  // namespace video_trimmer::event_handler

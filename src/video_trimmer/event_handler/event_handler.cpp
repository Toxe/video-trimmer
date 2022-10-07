#include "event_handler.hpp"

#include <unordered_map>

#include "SDL2/SDL.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"

#include "../logger/logger.hpp"
#include "events.hpp"

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
    commands_[Event::OpenFile] = no_command;
    commands_[Event::PlaybackStart] = no_command;
    commands_[Event::PlaybackTogglePause] = no_command;
    commands_[Event::JumpBackwardShort] = no_command;
    commands_[Event::JumpBackwardLong] = no_command;
    commands_[Event::JumpForwardShort] = no_command;
    commands_[Event::JumpForwardLong] = no_command;
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

        switch (event.type) {
            case SDL_QUIT: handle_event(Event::CloseWindow); break;
            case SDL_WINDOWEVENT: handle_window_events(event); break;
            case SDL_KEYDOWN: handle_keyboard_events(event); break;
        }
    }
}

void EventHandler::Impl::handle_window_events(const SDL_Event& event)
{
    switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED: handle_event(Event::ResizedWindow); break;
        case SDL_WINDOWEVENT_CLOSE: handle_event(Event::CloseWindow); break;
    }
}

void EventHandler::Impl::handle_keyboard_events(const SDL_Event& event)
{
    if (ImGui::GetIO().WantCaptureKeyboard)
        return;

    switch (event.key.keysym.sym) {
        case SDLK_ESCAPE: handle_event(Event::CloseWindow); break;
        case SDLK_SPACE: handle_event(Event::PlaybackTogglePause); break;
        case SDLK_F1: handle_event(Event::ToggleHelp); break;
        case SDLK_RIGHT: handle_event(Event::JumpForwardShort); break;
        case SDLK_LEFT: handle_event(Event::JumpBackwardShort); break;
        case SDLK_DOWN: handle_event(Event::JumpBackwardLong); break;
        case SDLK_UP: handle_event(Event::JumpForwardLong); break;
    }
}

EventHandler::EventHandler() : impl_(std::make_unique<EventHandler::Impl>()) { }
EventHandler::~EventHandler() = default;
void EventHandler::set_command(const Event& event, Command command) { impl_->set_command(event, command); }
void EventHandler::poll_events() { impl_->poll_events(); }
void EventHandler::handle_event(const Event& event) { impl_->handle_event(event); }

}  // namespace video_trimmer::event_handler

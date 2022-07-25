#include "playback_controls_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

#include "video_trimmer/event_handler/event_handler.hpp"
#include "video_trimmer/ui/colors/colors.hpp"
#include "video_trimmer/video_player/video_player.hpp"

namespace video_trimmer::views::playback_controls_view {

PlaybackControlsView::PlaybackControlsView(event_handler::EventHandler& event_handler)
    : event_handler_(event_handler)
{
}

void PlaybackControlsView::render(video_player::VideoPlayer& video_player)
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("playback controls");

    ImGui::TextUnformatted(fmt::format("playback controls [{}x{}]", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y).c_str());
    ImGui::TextUnformatted(fmt::format("playback position: {:.4f}", video_player.playback_position()).c_str());

    ImGui::TextColored(video_player.has_open_file() ? ui::colors::green : ui::colors::red, "%s", "has_open_file");
    ImGui::SameLine();
    ImGui::TextUnformatted("|");
    ImGui::SameLine();
    ImGui::TextColored(video_player.has_started_playing() ? ui::colors::green : ui::colors::red, "%s", "has_started_playing");
    ImGui::SameLine();
    ImGui::TextUnformatted("|");
    ImGui::SameLine();
    ImGui::TextColored(video_player.is_playing() ? ui::colors::green : ui::colors::red, "%s", "is_playing");
    ImGui::SameLine();
    ImGui::TextUnformatted("|");
    ImGui::SameLine();
    ImGui::TextColored(video_player.is_paused() ? ui::colors::green : ui::colors::red, "%s", "is_paused");
    ImGui::SameLine();

    if (video_player.has_open_file()) {
        if (!video_player.has_started_playing())
            if (ImGui::Button("start"))
                event_handler_.handle_event(event_handler::Event::PlaybackStart);

        if (video_player.is_playing())
            if (ImGui::Button("pause"))
                event_handler_.handle_event(event_handler::Event::PlaybackTogglePause);

        if (video_player.is_paused())
            if (ImGui::Button("resume"))
                event_handler_.handle_event(event_handler::Event::PlaybackTogglePause);
    }

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

}  // namespace video_trimmer::views::playback_controls_view

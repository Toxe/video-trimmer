#include "video_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

extern "C" {
#include "libavutil/frame.h"
}

#include "video_trimmer/ui/colors/colors.hpp"
#include "video_trimmer/ui/imgui_helpers.hpp"

namespace video_trimmer::views::video_view {

void VideoView::show_video_frame(video_trimmer::graphics::Graphics* graphics, std::unique_ptr<video_reader::frame::Frame> video_frame)
{
    if (video_frame) {
        current_video_frame_ = std::move(video_frame);

        if (!texture_ || !texture_->is_compatible_with_video_frame(current_video_frame_.get()))
            texture_ = std::make_unique<graphics::Texture>(graphics, current_video_frame_.get());

        if (texture_)
            texture_->update(current_video_frame_.get());
    }
}

void VideoView::render(video_trimmer::graphics::Graphics* graphics)
{
    render_ui();
    render_content(graphics);
}

void VideoView::render_ui()
{
    ImGui::Begin("Video Trimmer");
    ImGui::BeginChild("right pane");
    ImGui::BeginChild("video");

    // current view position and size
    const ImVec2 imgui_cursor_screen_pos = ImGui::GetCursorScreenPos();
    const ImVec2 imgui_window_size = ImGui::GetWindowSize();

    view_position_ = Position{static_cast<int>(imgui_cursor_screen_pos.x), static_cast<int>(imgui_cursor_screen_pos.y)};
    view_size_ = Size{static_cast<int>(imgui_window_size.x), static_cast<int>(imgui_window_size.y)};

    video_trimmer::ui::imgui_text_outlined(video_trimmer::ui::colors::white, video_trimmer::ui::colors::black, fmt::format("video [{}x{}]", view_size_.width, view_size_.height));

    if (current_video_frame_)
        video_trimmer::ui::imgui_text_outlined(video_trimmer::ui::colors::white, video_trimmer::ui::colors::black, fmt::format("[VideoFrame {:.4f}, {}x{}]", current_video_frame_->timestamp(), current_video_frame_->size().width, current_video_frame_->size().height));

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

void VideoView::render_content(video_trimmer::graphics::Graphics* graphics)
{
    if (texture_)
        texture_->draw(graphics, view_position_, view_size_);
}

}  // namespace video_trimmer::views::video_view

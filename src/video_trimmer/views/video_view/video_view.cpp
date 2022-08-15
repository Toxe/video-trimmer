#include "video_view.hpp"

#include "fmt/core.h"
#include "imgui.h"

extern "C" {
#include "libavutil/frame.h"
}

#include "../../ui/colors/colors.hpp"
#include "../../ui/imgui_helpers.hpp"

namespace video_trimmer::views::video_view {

void VideoView::show_video_frame(graphics::Graphics& graphics, std::unique_ptr<video_file::Frame> video_frame)
{
    if (video_frame) {
        current_video_frame_ = std::move(video_frame);

        create_compatible_render_texture_if_necessary(graphics, current_video_frame_->size(), current_video_frame_->pixel_format());

        if (texture_)
            texture_->update(current_video_frame_.get());
    }
}

void VideoView::create_compatible_render_texture_if_necessary(graphics::Graphics& graphics, Size size, video_file::PixelFormat pixel_format)
{
    if (!texture_ || !texture_->is_compatible(size, pixel_format))
        texture_ = std::make_unique<graphics::Texture>(graphics, size, pixel_format);
}

void VideoView::render(graphics::Graphics& graphics)
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

    ui::imgui_text_outlined(ui::colors::white, ui::colors::black, fmt::format("video [{}x{}]", view_size_.width, view_size_.height));

    if (current_video_frame_)
        ui::imgui_text_outlined(ui::colors::white, ui::colors::black, current_video_frame_->description());

    ImGui::EndChild();
    ImGui::EndChild();
    ImGui::End();
}

void VideoView::render_content(graphics::Graphics& graphics)
{
    if (texture_)
        texture_->draw(graphics, view_position_, view_size_);
}

}  // namespace video_trimmer::views::video_view

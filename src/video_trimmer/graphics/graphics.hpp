#pragma once

#include <memory>

#include "../clock/clock.hpp"
#include "types.hpp"

struct SDL_Texture;

namespace video_trimmer::graphics {

class Graphics {
public:
    Graphics();
    ~Graphics();

    void init_sdl();
    void init_imgui(int font_size);

    void shutdown_sdl();
    void shutdown_imgui();

    void create_window(const char* title, Size size);
    void create_renderer(bool disable_vsync);

    void begin_frame();
    clock::Duration finish_frame(const clock::Clock& frame_time_clock);

    [[nodiscard]] bool window_is_open() const;
    [[nodiscard]] Size window_size() const;

    [[nodiscard]] SDL_Texture* create_texture(uint32_t format, Size size);
    void render_texture(SDL_Texture* texture, Position dst_position, Size dst_size);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

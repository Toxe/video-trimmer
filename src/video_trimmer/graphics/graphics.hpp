#pragma once

#include <memory>

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

    void create_window(const char* title, WindowSize size);
    void create_renderer(bool disable_vsync);

    void begin_frame();
    void finish_frame();

    [[nodiscard]] bool window_is_open() const;
    [[nodiscard]] WindowSize window_size() const;

    [[nodiscard]] SDL_Texture* create_texture(uint32_t format, ImageSize size);
    void render_texture(SDL_Texture* texture, ImagePosition dst_position, ImageSize dst_size);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

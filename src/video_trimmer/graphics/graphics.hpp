#pragma once

#include <memory>

#include "types.hpp"

struct SDL_Texture;

namespace video_trimmer::graphics {

class Texture;

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

    [[nodiscard]] std::unique_ptr<Texture> create_texture(ImageSize size);
    static void destroy_texture(Texture* texture);
    void update_texture(Texture* texture, const void* pixels);
    void draw_texture(Texture* texture, ImagePosition dst_position, ImageSize dst_size);

    [[nodiscard]] bool window_is_open() const;
    [[nodiscard]] WindowSize window_size() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::graphics

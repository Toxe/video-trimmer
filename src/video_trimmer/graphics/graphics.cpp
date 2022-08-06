#include "graphics.hpp"

#include <cassert>
#include <cmath>
#include <string>
#include <tuple>

#include "glad/glad.h"

#include "SDL2/SDL.h"
#include "fmt/core.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "video_trimmer/error/error.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::graphics {

class Graphics::Impl {
public:
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
    std::string glsl_version_;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;

    [[nodiscard]] std::tuple<Size, bool, float> get_display_size() const;
    [[nodiscard]] int get_default_font_size() const;
    [[nodiscard]] Size get_default_window_size() const;
};

void Graphics::Impl::init_sdl()
{
    logger::log_info(fmt::format("SDL version: {}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        error::die(fmt::format("unable to initialize SDL: {}", SDL_GetError()));

// decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version_ = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glsl_version_ = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);  // always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    glsl_version_ = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif
}

void Graphics::Impl::init_imgui(int font_size)
{
    if (font_size <= 0)
        font_size = get_default_font_size();

    logger::log_info(fmt::format("font size: {} pixels", font_size));

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("assets/fonts/Inconsolata-SemiBold.ttf", static_cast<float>(font_size));

    // setup platform/renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window_, gl_context_);
    ImGui_ImplOpenGL3_Init(glsl_version_.c_str());
}

void Graphics::Impl::shutdown_sdl()
{
    SDL_GL_DeleteContext(gl_context_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();

    window_ = nullptr;
    renderer_ = nullptr;
    gl_context_ = nullptr;
}

void Graphics::Impl::shutdown_imgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Graphics::Impl::create_window(const char* title, Size size)
{
    if (size.width <= 0 || size.height <= 0)
        size = get_default_window_size();

    // create main window with graphics context
    logger::log_info(fmt::format("open main window: {}x{}", size.width, size.height));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    const auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, window_flags);

    if (!window_)
        error::die(fmt::format("unable to create window: {}", SDL_GetError()));

    gl_context_ = SDL_GL_CreateContext(window_);
    SDL_GL_MakeCurrent(window_, gl_context_);
    SDL_GL_SetSwapInterval(1);
}

void Graphics::Impl::create_renderer(bool disable_vsync)
{
    assert(window_);

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");

    Uint32 renderer_flags = SDL_RENDERER_ACCELERATED;

    if (!disable_vsync)
        renderer_flags |= SDL_RENDERER_PRESENTVSYNC;

    renderer_ = SDL_CreateRenderer(window_, -1, renderer_flags);

    if (!renderer_)
        error::die(fmt::format("unable to create renderer: {}", SDL_GetError()));

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer_, &info);

    logger::log_info(fmt::format("renderer: {}", info.name));

    // load GL extensions using glad
    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        error::die("Failed to initialize the OpenGL context");

    // loaded OpenGL successfully
    logger::log_info(fmt::format("OpenGL version loaded: {}.{}", GLVersion.major, GLVersion.minor));
}

void Graphics::Impl::begin_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    const ImGuiIO& io = ImGui::GetIO();

    glViewport(0, 0, static_cast<int>(io.DisplaySize.x), static_cast<int>(io.DisplaySize.y));
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

clock::Duration Graphics::Impl::finish_frame(const clock::Clock& frame_time_clock)
{
    assert(window_);

    ImGui::Render();

    // measure frame time right before VSync
    const clock::Duration frame_time = frame_time_clock.elapsed_time();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window_);

    return frame_time;
}

bool Graphics::Impl::window_is_open() const
{
    if (!window_)
        return false;

    return SDL_GetWindowFlags(window_) & SDL_WINDOW_SHOWN;
}

Size Graphics::Impl::window_size() const
{
    assert(window_);

    Size size{};
    SDL_GetWindowSize(window_, &size.width, &size.height);

    return size;
}

std::tuple<Size, bool, float> Graphics::Impl::get_display_size() const
{
    SDL_DisplayMode current_display_mode;
    SDL_DisplayMode desktop_display_mode;

    if (SDL_GetDisplayMode(0, 0, &current_display_mode) < 0)
        error::die(fmt::format("unable to get display mode: {}", SDL_GetError()));

    if (SDL_GetDesktopDisplayMode(0, &desktop_display_mode) < 0)
        error::die(fmt::format("unable to get desktop display mode: {}", SDL_GetError()));

    const bool display_is_scaled = current_display_mode.h != desktop_display_mode.h;
    const float scale_factor = static_cast<float>(current_display_mode.h) / static_cast<float>(desktop_display_mode.h);

    return {Size{current_display_mode.w, current_display_mode.h}, display_is_scaled, scale_factor};
}

int Graphics::Impl::get_default_font_size() const
{
    const auto [display_size, display_is_scaled, scale_factor] = get_display_size();

    return display_size.height / 108;
}

Size Graphics::Impl::get_default_window_size() const
{
    const auto [display_size, display_is_scaled, scale_factor] = get_display_size();

    logger::log_info(fmt::format("current display size: {}x{} (scale factor: {})", display_size.width, display_size.height, scale_factor));

    // 80% desktop height and 16:9 aspect ratio
    if (display_is_scaled) {
        // need to compensate desktop scale factor
        const float height = std::round(static_cast<float>(display_size.height) * 0.8f / scale_factor);
        const float width = std::round(16.0f * height / 9.0f);
        return {static_cast<int>(width), static_cast<int>(height)};
    }

    const float height = std::round(static_cast<float>(display_size.height) * 0.8f);
    const float width = std::round(16.0f * height / 9.0f);
    return {static_cast<int>(width), static_cast<int>(height)};
}

SDL_Texture* Graphics::Impl::create_texture(uint32_t format, Size size)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer_, format, SDL_TEXTUREACCESS_STREAMING, size.width, size.height);

    if (!texture) {
        logger::log_error(fmt::format("unable to create texture: {}", SDL_GetError()));
        return nullptr;
    }

    return texture;
}

void Graphics::Impl::render_texture(SDL_Texture* texture, Position dst_position, Size dst_size)
{
    assert(texture);

    SDL_Rect dst_rect{dst_position.x, dst_position.y, dst_size.width, dst_size.height};
    SDL_RenderCopy(renderer_, texture, nullptr, &dst_rect);
}

Graphics::Graphics() : impl_(std::make_unique<Graphics::Impl>()) { }
Graphics::~Graphics() = default;
void Graphics::init_sdl() { impl_->init_sdl(); }
void Graphics::init_imgui(int font_size) { impl_->init_imgui(font_size); }
void Graphics::shutdown_sdl() { impl_->shutdown_sdl(); }
void Graphics::shutdown_imgui() { impl_->shutdown_imgui(); }
void Graphics::create_window(const char* title, Size size) { impl_->create_window(title, size); }
void Graphics::create_renderer(bool disable_vsync) { impl_->create_renderer(disable_vsync); }
void Graphics::begin_frame() { impl_->begin_frame(); }
clock::Duration Graphics::finish_frame(const clock::Clock& frame_time_clock) { return impl_->finish_frame(frame_time_clock); }
bool Graphics::window_is_open() const { return impl_->window_is_open(); }
Size Graphics::window_size() const { return impl_->window_size(); }
SDL_Texture* Graphics::create_texture(uint32_t format, Size size) { return impl_->create_texture(format, size); }
void Graphics::render_texture(SDL_Texture* texture, Position dst_position, Size dst_size) { impl_->render_texture(texture, dst_position, dst_size); }

}  // namespace video_trimmer::graphics

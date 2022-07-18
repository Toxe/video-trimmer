#include "graphics.hpp"

#include <cassert>
#include <string>

#include "glad/glad.h"

#include "SDL2/SDL.h"
#include "fmt/core.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "texture.hpp"
#include "video_trimmer/error/error.hpp"
#include "video_trimmer/logger/logger.hpp"

namespace video_trimmer::graphics {

class Graphics::Impl {
public:
    void init_sdl();
    void init_imgui(int font_size);

    void shutdown_sdl();
    void shutdown_imgui();

    void create_window(const char* title, WindowSize size);

    void begin_frame();
    void finish_frame();

    [[nodiscard]] bool window_is_open() const;
    [[nodiscard]] WindowSize window_size() const;

    [[nodiscard]] std::unique_ptr<Texture> create_texture(ImageSize size);
    static void destroy_texture(Texture* texture);
    void update_texture(Texture* texture, const void* pixels);
    void draw_texture(Texture* texture, ImagePosition dst_position, ImageSize dst_size);

private:
    std::string glsl_version_;

    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;

    [[nodiscard]] SDL_DisplayMode get_current_display_mode() const;
    [[nodiscard]] int get_default_font_size() const;
    [[nodiscard]] WindowSize get_default_window_size() const;
};

void Graphics::Impl::init_sdl()
{
    video_trimmer::logger::log_info(fmt::format("SDL v{}.{}.{}", SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        video_trimmer::error::die(fmt::format("unable to initialize SDL: {}", SDL_GetError()));

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

void Graphics::Impl::create_window(const char* title, WindowSize size)
{
    if (size.width <= 0 || size.height <= 0)
        size = get_default_window_size();

    // create main window with graphics context
    video_trimmer::logger::log_info(fmt::format("SDL init main window {}x{}", size.width, size.height));

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    const auto window_flags = static_cast<SDL_WindowFlags>(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    window_ = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.width, size.height, window_flags);

    if (!window_)
        video_trimmer::error::die(fmt::format("unable to create window: {}", SDL_GetError()));

    gl_context_ = SDL_GL_CreateContext(window_);
    SDL_GL_MakeCurrent(window_, gl_context_);
    SDL_GL_SetSwapInterval(1);

    // create renderer
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer_)
        video_trimmer::error::die(fmt::format("unable to create renderer: {}", SDL_GetError()));

    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer_, &info);

    video_trimmer::logger::log_info(fmt::format("SDL renderer: {}", info.name));

    // load GL extensions using glad
    if (!gladLoadGLLoader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        video_trimmer::error::die("Failed to initialize the OpenGL context");

    // loaded OpenGL successfully
    video_trimmer::logger::log_info(fmt::format("OpenGL version loaded: {}.{}", GLVersion.major, GLVersion.minor));
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

void Graphics::Impl::finish_frame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    SDL_GL_SwapWindow(window_);
}

bool Graphics::Impl::window_is_open() const
{
    return SDL_GetWindowFlags(window_) & SDL_WINDOW_SHOWN;
}

WindowSize Graphics::Impl::window_size() const
{
    WindowSize size{};
    SDL_GetWindowSize(window_, &size.width, &size.height);

    return size;
}

SDL_DisplayMode Graphics::Impl::get_current_display_mode() const
{
    SDL_DisplayMode mode;

    // if (SDL_GetDesktopDisplayMode(0, &mode) < 0)
    //     video_trimmer::error::die(fmt::format("unable to get desktop display mode: {}", SDL_GetError()));

    if (SDL_GetDisplayMode(0, 0, &mode) < 0)
        video_trimmer::error::die(fmt::format("unable to get display mode: {}", SDL_GetError()));

    return mode;
}

int Graphics::Impl::get_default_font_size() const
{
    return get_current_display_mode().h / 108;
}

WindowSize Graphics::Impl::get_default_window_size() const
{
    // 50% desktop height and 16:9 aspect ratio
    const int height = get_current_display_mode().h / 2;
    const int width = 16 * height / 9;

    return {width, height};
}

std::unique_ptr<Texture> Graphics::Impl::create_texture(ImageSize size)
{
    SDL_Texture* texture = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, size.width, size.height);

    if (!texture) {
        video_trimmer::logger::log_error(fmt::format("unable to create texture: {}", SDL_GetError()));
        return nullptr;
    }

    return std::make_unique<Texture>(texture, size);
}

void Graphics::Impl::destroy_texture(Texture* texture)
{
    assert(texture && texture->sdl_texture());

    SDL_DestroyTexture(texture->sdl_texture());
}

void Graphics::Impl::update_texture(Texture* texture, const void* pixels)
{
    assert(texture && pixels);

    if (SDL_UpdateTexture(texture->sdl_texture(), nullptr, pixels, 4 * texture->size().width) < 0)
        video_trimmer::logger::log_error(fmt::format("unable to update texture: {}", SDL_GetError()));
}

void Graphics::Impl::draw_texture(Texture* texture, ImagePosition dst_position, ImageSize dst_size)
{
    assert(texture);

    SDL_Rect dst_rect{dst_position.x, dst_position.y, dst_size.width, dst_size.height};
    SDL_RenderCopy(renderer_, texture->sdl_texture(), nullptr, &dst_rect);
}

Graphics::Graphics() : impl_(std::make_unique<Graphics::Impl>()) { }
Graphics::~Graphics() = default;
void Graphics::init_sdl() { impl_->init_sdl(); }
void Graphics::init_imgui(int font_size) { impl_->init_imgui(font_size); }
void Graphics::shutdown_sdl() { impl_->shutdown_sdl(); }
void Graphics::shutdown_imgui() { impl_->shutdown_imgui(); }
void Graphics::create_window(const char* title, WindowSize size) { impl_->create_window(title, size); }
void Graphics::begin_frame() { impl_->begin_frame(); }
void Graphics::finish_frame() { impl_->finish_frame(); }
bool Graphics::window_is_open() const { return impl_->window_is_open(); }
WindowSize Graphics::window_size() const { return impl_->window_size(); }
std::unique_ptr<Texture> Graphics::create_texture(ImageSize size) { return impl_->create_texture(size); }
void Graphics::destroy_texture(Texture* texture) { Graphics::Impl::destroy_texture(texture); }
void Graphics::update_texture(Texture* texture, const void* pixels) { impl_->update_texture(texture, pixels); }
void Graphics::draw_texture(Texture* texture, ImagePosition dst_position, ImageSize dst_size) { impl_->draw_texture(texture, dst_position, dst_size); }

}  // namespace video_trimmer::graphics

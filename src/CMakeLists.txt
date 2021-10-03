add_executable(video_trimmer
    main.cpp
    register_events.cpp register_events.h
    app/app.cpp app/app.h
    clock/clock.h
    clock/duration.h
    clock/stopwatch.h
    command_line/command_line.cpp command_line/command_line.h
    event_handler/event_handler.cpp event_handler/event_handler.h
    event_handler/events.h
    include/types.h
    ui/colors.h
    ui/commands.h
    ui/fps_view.cpp ui/fps_view.h
    ui/input_value.h
    ui/ui.cpp ui/ui.h
    video_view/video_view.cpp video_view/video_view.h
    window/commands.h
    window/window.cpp window/window.h
)

set_target_properties(video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(video_trimmer PUBLIC cxx_std_20)
target_compile_options(video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_include_directories(video_trimmer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include)
target_link_libraries(video_trimmer PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 fmt::fmt spdlog::spdlog spdlog::spdlog_header_only sfml-system sfml-graphics sfml-window sfml-audio ImGui-SFML::ImGui-SFML)

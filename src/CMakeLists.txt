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
    ui/ui.cpp ui/ui.h
    ui/views/fps_view.cpp ui/views/fps_view.h
    ui/views/video_file_info_view.cpp ui/views/video_file_info_view.hpp
    video_view/video_view.cpp video_view/video_view.h
    video/auto_delete_ressource.hpp
    video/stream_info.hpp
    video/video_file.cpp video/video_file.hpp
    video/video_stream.cpp video/video_stream.hpp
    window/commands.h
    window/window.cpp window/window.h
)

set_target_properties(video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(video_trimmer PUBLIC cxx_std_20)
target_compile_options(video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_include_directories(video_trimmer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include ${FFMPEG_INCLUDE_DIRS})
target_link_directories(video_trimmer PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(video_trimmer PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 fmt::fmt spdlog::spdlog spdlog::spdlog_header_only sfml-system sfml-graphics sfml-window sfml-audio ImGui-SFML::ImGui-SFML ${FFMPEG_LIBRARIES})

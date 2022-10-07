if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(
            LINUX_SOURCES
            ui/widgets/memory_usage_widget/memory_usage_info/linux_memory_usage_info.cpp
            ui/widgets/memory_usage_widget/memory_usage_info/linux_memory_usage_info.hpp
    )
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(
            MAC_SOURCES
            ui/widgets/memory_usage_widget/memory_usage_info/mac_memory_usage_info.cpp
            ui/widgets/memory_usage_widget/memory_usage_info/mac_memory_usage_info.hpp
    )
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(
            WINDOWS_SOURCES
            ui/widgets/memory_usage_widget/memory_usage_info/windows_memory_usage_info.cpp
            ui/widgets/memory_usage_widget/memory_usage_info/windows_memory_usage_info.hpp
    )
endif ()

add_library(LIB_video_trimmer
        app/app.cpp
        app/app.hpp
        app/system_info.cpp
        app/system_info.hpp
        clock/clock.hpp
        clock/duration.hpp
        clock/stopwatch.hpp
        command_line/command_line.cpp
        command_line/command_line.hpp
        error/error.cpp
        error/error.hpp
        event_handler/command.hpp
        event_handler/event_handler.cpp
        event_handler/event_handler.hpp
        event_handler/events.hpp
        event_handler/register_events.cpp
        event_handler/register_events.hpp
        graphics/graphics.cpp
        graphics/graphics.hpp
        graphics/texture.cpp
        graphics/texture.hpp
        include/auto_delete_resource.hpp
        include/types.hpp
        logger/logger.cpp
        logger/logger.hpp
        main_window/commands.cpp
        main_window/commands.hpp
        main_window/main_window.cpp
        main_window/main_window.hpp
        ui/colors/colors.hpp
        ui/commands.cpp
        ui/commands.hpp
        ui/imgui_helpers.cpp
        ui/imgui_helpers.hpp
        ui/ui.cpp
        ui/ui.hpp
        ui/widgets/fps_widget/fps_widget.cpp
        ui/widgets/fps_widget/fps_widget.hpp
        ui/widgets/frame_time_widget/frame_time_widget.cpp
        ui/widgets/frame_time_widget/frame_time_widget.hpp
        ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.cpp
        ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.hpp
        ui/widgets/memory_usage_widget/memory_usage_info/memory_usage_info.hpp
        ui/widgets/memory_usage_widget/memory_usage_widget.cpp
        ui/widgets/memory_usage_widget/memory_usage_widget.hpp
        ui/widgets/ring_buffer.cpp
        ui/widgets/ring_buffer.hpp
        ui/widgets/video_file_info_widget/video_file_info_widget.cpp
        ui/widgets/video_file_info_widget/video_file_info_widget.hpp
        video_file/codec_context.cpp
        video_file/codec_context.hpp
        video_file/frame.cpp
        video_file/frame.hpp
        video_file/pixel_format.cpp
        video_file/pixel_format.hpp
        video_file/video_file.cpp
        video_file/video_file.hpp
        video_player/commands.cpp
        video_player/commands.hpp
        video_player/video_player.cpp
        video_player/video_player.hpp
        views/additional_info_view/additional_info_view.cpp
        views/additional_info_view/additional_info_view.hpp
        views/files_view/directory_scanner.cpp
        views/files_view/directory_scanner.hpp
        views/files_view/file_entry.cpp
        views/files_view/file_entry.hpp
        views/files_view/files_view.cpp
        views/files_view/files_view.hpp
        views/playback_controls_view/playback_controls_view.cpp
        views/playback_controls_view/playback_controls_view.hpp
        views/trim_controls_view/trim_controls_view.cpp
        views/trim_controls_view/trim_controls_view.hpp
        views/video_view/video_view.cpp
        views/video_view/video_view.hpp
        ${LINUX_SOURCES}
        ${MAC_SOURCES}
        ${WINDOWS_SOURCES}
        )

set_target_properties(LIB_video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(LIB_video_trimmer PUBLIC cxx_std_20)
target_compile_options(LIB_video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_include_directories(LIB_video_trimmer PUBLIC include ${FFMPEG_INCLUDE_DIRS})
target_link_directories(LIB_video_trimmer PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(LIB_video_trimmer PRIVATE ${SANITIZER_FLAGS} LIB_ImGuiFileDialog CLI11::CLI11 fmt::fmt spdlog::spdlog glad::glad SDL2::SDL2 imgui::imgui ${FFMPEG_LIBRARIES})

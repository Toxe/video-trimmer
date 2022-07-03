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

add_executable(video_trimmer
        main.cpp
        app/app.cpp
        app/app.h
        clock/clock.h
        clock/duration.h
        clock/stopwatch.h
        command_line/command_line.cpp
        command_line/command_line.h
        error/error.cpp
        error/error.hpp
        event_handler/event_handler.cpp
        event_handler/event_handler.h
        event_handler/events.h
        include/auto_delete_resource.hpp
        include/types.h
        logger/logger.cpp
        logger/logger.hpp
        main_window/commands.cpp
        main_window/commands.h
        main_window/main_window.cpp
        main_window/main_window.hpp
        register_events.cpp
        register_events.h
        threaded_stream_reader/codec_context/codec_context.cpp
        threaded_stream_reader/codec_context/codec_context.hpp
        threaded_stream_reader/format_context/format_context.cpp
        threaded_stream_reader/format_context/format_context.hpp
        threaded_stream_reader/frame/frame.cpp
        threaded_stream_reader/frame/frame.hpp
        threaded_stream_reader/packet/packet.cpp
        threaded_stream_reader/packet/packet.hpp
        threaded_stream_reader/scaling_context/scaling_context.cpp
        threaded_stream_reader/scaling_context/scaling_context.hpp
        threaded_stream_reader/stream_info/stream_info.cpp
        threaded_stream_reader/stream_info/stream_info.hpp
        threaded_stream_reader/video_content_provider/run_state.hpp
        threaded_stream_reader/video_content_provider/video_content_provider.cpp
        threaded_stream_reader/video_content_provider/video_content_provider.hpp
        threaded_stream_reader/video_content_provider/video_frame_queue.cpp
        threaded_stream_reader/video_content_provider/video_frame_queue.hpp
        threaded_stream_reader/video_content_provider/video_frame_scaler.cpp
        threaded_stream_reader/video_content_provider/video_frame_scaler.hpp
        threaded_stream_reader/video_content_provider/video_reader.cpp
        threaded_stream_reader/video_content_provider/video_reader.hpp
        threaded_stream_reader/video_content_provider/work_thread.cpp
        threaded_stream_reader/video_content_provider/work_thread.hpp
        threaded_stream_reader/video_file.cpp
        threaded_stream_reader/video_file.hpp
        threaded_stream_reader/video_frame/texture.cpp
        threaded_stream_reader/video_frame/texture.hpp
        threaded_stream_reader/video_frame/video_frame.cpp
        threaded_stream_reader/video_frame/video_frame.hpp
        ui/colors.h
        ui/commands.cpp
        ui/commands.h
        ui/imgui_helpers.cpp
        ui/imgui_helpers.hpp
        ui/ui.cpp
        ui/ui.h
        ui/widgets/fps_widget.cpp
        ui/widgets/fps_widget.hpp
        ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.cpp
        ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.hpp
        ui/widgets/memory_usage_widget/memory_usage_info/memory_usage_info.hpp
        ui/widgets/memory_usage_widget/memory_usage_widget.cpp
        ui/widgets/memory_usage_widget/memory_usage_widget.hpp
        ui/widgets/video_file_info_widget.cpp
        ui/widgets/video_file_info_widget.hpp
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
        views/video_view/video_view.h
        ${LINUX_SOURCES}
        ${MAC_SOURCES}
        ${WINDOWS_SOURCES}
        )

set_target_properties(video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(video_trimmer PUBLIC cxx_std_20)
target_compile_options(video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_include_directories(video_trimmer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} include ${FFMPEG_INCLUDE_DIRS})
target_link_directories(video_trimmer PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(video_trimmer PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 fmt::fmt spdlog::spdlog sfml-system sfml-graphics sfml-window sfml-audio ImGui-SFML::ImGui-SFML ${FFMPEG_LIBRARIES})

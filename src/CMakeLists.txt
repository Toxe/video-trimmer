if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(
            LINUX_SOURCES
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/linux_memory_usage_info.cpp
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/linux_memory_usage_info.hpp
    )
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(
            MAC_SOURCES
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/mac_memory_usage_info.cpp
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/mac_memory_usage_info.hpp
    )
endif ()

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(
            WINDOWS_SOURCES
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/windows_memory_usage_info.cpp
            video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/windows_memory_usage_info.hpp
    )
endif ()

add_executable(video_trimmer
        main.cpp
        video_trimmer/app/app.cpp
        video_trimmer/app/app.hpp
        video_trimmer/clock/clock.hpp
        video_trimmer/clock/duration.hpp
        video_trimmer/clock/stopwatch.hpp
        video_trimmer/command_line/command_line.cpp
        video_trimmer/command_line/command_line.h
        video_trimmer/error/error.cpp
        video_trimmer/error/error.hpp
        video_trimmer/event_handler/command.hpp
        video_trimmer/event_handler/event_handler.cpp
        video_trimmer/event_handler/event_handler.hpp
        video_trimmer/event_handler/events.hpp
        video_trimmer/event_handler/register_events.cpp
        video_trimmer/event_handler/register_events.hpp
        video_trimmer/include/auto_delete_resource.hpp
        video_trimmer/include/types.h
        video_trimmer/logger/logger.cpp
        video_trimmer/logger/logger.hpp
        video_trimmer/main_window/commands.cpp
        video_trimmer/main_window/commands.hpp
        video_trimmer/main_window/main_window.cpp
        video_trimmer/main_window/main_window.hpp
        video_trimmer/threaded_stream_reader/codec_context/codec_context.cpp
        video_trimmer/threaded_stream_reader/codec_context/codec_context.hpp
        video_trimmer/threaded_stream_reader/format_context/format_context.cpp
        video_trimmer/threaded_stream_reader/format_context/format_context.hpp
        video_trimmer/threaded_stream_reader/frame/frame.cpp
        video_trimmer/threaded_stream_reader/frame/frame.hpp
        video_trimmer/threaded_stream_reader/packet/packet.cpp
        video_trimmer/threaded_stream_reader/packet/packet.hpp
        video_trimmer/threaded_stream_reader/scaling_context/scaling_context.cpp
        video_trimmer/threaded_stream_reader/scaling_context/scaling_context.hpp
        video_trimmer/threaded_stream_reader/stream_info/stream_info.cpp
        video_trimmer/threaded_stream_reader/stream_info/stream_info.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/run_state.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_content_provider.cpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_content_provider.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_frame_queue.cpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_frame_queue.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_frame_scaler.cpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_frame_scaler.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_reader.cpp
        video_trimmer/threaded_stream_reader/video_content_provider/video_reader.hpp
        video_trimmer/threaded_stream_reader/video_content_provider/work_thread.cpp
        video_trimmer/threaded_stream_reader/video_content_provider/work_thread.hpp
        video_trimmer/threaded_stream_reader/video_file.cpp
        video_trimmer/threaded_stream_reader/video_file.hpp
        video_trimmer/threaded_stream_reader/video_frame/texture.cpp
        video_trimmer/threaded_stream_reader/video_frame/texture.hpp
        video_trimmer/threaded_stream_reader/video_frame/video_frame.cpp
        video_trimmer/threaded_stream_reader/video_frame/video_frame.hpp
        video_trimmer/ui/colors/colors.hpp
        video_trimmer/ui/commands.cpp
        video_trimmer/ui/commands.hpp
        video_trimmer/ui/imgui_helpers.cpp
        video_trimmer/ui/imgui_helpers.hpp
        video_trimmer/ui/ui.cpp
        video_trimmer/ui/ui.hpp
        video_trimmer/ui/widgets/fps_widget/fps_widget.cpp
        video_trimmer/ui/widgets/fps_widget/fps_widget.hpp
        video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.cpp
        video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/create_memory_usage_info.hpp
        video_trimmer/ui/widgets/memory_usage_widget/memory_usage_info/memory_usage_info.hpp
        video_trimmer/ui/widgets/memory_usage_widget/memory_usage_widget.cpp
        video_trimmer/ui/widgets/memory_usage_widget/memory_usage_widget.hpp
        video_trimmer/ui/widgets/video_file_info_widget/video_file_info_widget.cpp
        video_trimmer/ui/widgets/video_file_info_widget/video_file_info_widget.hpp
        video_trimmer/video_player/video_player.cpp
        video_trimmer/video_player/video_player.hpp
        video_trimmer/views/additional_info_view/additional_info_view.cpp
        video_trimmer/views/additional_info_view/additional_info_view.hpp
        video_trimmer/views/files_view/directory_scanner.cpp
        video_trimmer/views/files_view/directory_scanner.hpp
        video_trimmer/views/files_view/file_entry.cpp
        video_trimmer/views/files_view/file_entry.hpp
        video_trimmer/views/files_view/files_view.cpp
        video_trimmer/views/files_view/files_view.hpp
        video_trimmer/views/playback_controls_view/playback_controls_view.cpp
        video_trimmer/views/playback_controls_view/playback_controls_view.hpp
        video_trimmer/views/trim_controls_view/trim_controls_view.cpp
        video_trimmer/views/trim_controls_view/trim_controls_view.hpp
        video_trimmer/views/video_view/video_view.cpp
        video_trimmer/views/video_view/video_view.h
        ${LINUX_SOURCES}
        ${MAC_SOURCES}
        ${WINDOWS_SOURCES}
        )

set_target_properties(video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(video_trimmer PUBLIC cxx_std_20)
target_compile_options(video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS_AND_WARNINGS})
target_include_directories(video_trimmer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} video_trimmer/include ${FFMPEG_INCLUDE_DIRS})
target_link_directories(video_trimmer PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(video_trimmer PRIVATE ${SANITIZER_FLAGS} CLI11::CLI11 fmt::fmt spdlog::spdlog sfml-system sfml-graphics sfml-window sfml-audio ImGui-SFML::ImGui-SFML ${FFMPEG_LIBRARIES})

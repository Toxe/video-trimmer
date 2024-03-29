add_subdirectory(lib)
add_subdirectory(video_trimmer)

add_executable(video_trimmer main.cpp)

set_target_properties(video_trimmer PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(video_trimmer PUBLIC cxx_std_20)
target_compile_options(video_trimmer PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
target_link_libraries(video_trimmer PRIVATE ${SANITIZER_FLAGS} LIB_video_trimmer SDL2::SDL2main)

# ---- Tests ----
if (BUILD_TESTING)
    add_executable(video_trimmer_tests
            video_trimmer/clock/fmt.test.cpp
            video_trimmer/clock/frame_duration.test.cpp
            video_trimmer/clock/playback_clock.test.cpp
            video_trimmer/clock/playback_position.test.cpp
            video_trimmer/clock/playback_time_point.test.cpp
            video_trimmer/clock/video_duration.test.cpp
            video_trimmer/command_line/command_line.test.cpp
            video_trimmer/ui/widgets/ring_buffer.test.cpp
            video_trimmer/video_file/frame.test.cpp
            video_trimmer/video_file/pixel_format.test.cpp
            video_trimmer/video_player/video_player.test.cpp
            )

    set_target_properties(video_trimmer_tests PROPERTIES CXX_EXTENSIONS OFF)
    target_compile_features(video_trimmer_tests PUBLIC cxx_std_20)
    target_compile_options(video_trimmer_tests PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS})
    target_include_directories(video_trimmer_tests PRIVATE ${FAKEIT_INCLUDE_DIRS})
    target_link_libraries(video_trimmer_tests PRIVATE ${SANITIZER_FLAGS} LIB_video_trimmer Catch2::Catch2WithMain)

    catch_discover_tests(video_trimmer_tests)
endif ()

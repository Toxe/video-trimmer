# deactivate all warnings
add_library(LIB_ImGuiFileDialog
        ImGuiFileDialog/ImGuiFileDialog.cpp
        ImGuiFileDialog/ImGuiFileDialog.h
        ImGuiFileDialog/ImGuiFileDialogConfig.h
        ImGuiFileDialog/dirent/dirent.h
        ImGuiFileDialog/stb/stb_image.h
        ImGuiFileDialog/stb/stb_image_resize.h
        )

set_target_properties(LIB_ImGuiFileDialog PROPERTIES CXX_EXTENSIONS OFF)
target_compile_features(LIB_ImGuiFileDialog PUBLIC cxx_std_20)
target_compile_options(LIB_ImGuiFileDialog PRIVATE ${SANITIZER_FLAGS} ${DEFAULT_COMPILER_OPTIONS} ${DEFAULT_COMPILER_WARNINGS_DISABLED})
target_link_libraries(LIB_ImGuiFileDialog PRIVATE ${SANITIZER_FLAGS} imgui::imgui)

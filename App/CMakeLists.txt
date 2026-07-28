project(MerlinApp)

find_package(imgui CONFIG REQUIRED)
find_path(PORTABLE_FILE_DIALOGS_INCLUDE_DIRS "portable-file-dialogs.h")

# this is an example application that uses Merlin
add_executable(MerlinApp
        src/main.cpp
        src/Application.cpp
        src/Application.h
        src/GUI/GuiModule.cpp
        src/GUI/GuiModule.h
)

target_link_libraries(MerlinApp PRIVATE
        Merlin
        imgui::imgui
)

target_include_directories(MerlinApp PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${PORTABLE_FILE_DIALOGS_INCLUDE_DIRS}
)

# todo: figure this shader part out
target_compile_definitions(MerlinApp PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/Merlin/src/shaders")
target_precompile_headers(MerlinApp PRIVATE src/gui_pch.h)

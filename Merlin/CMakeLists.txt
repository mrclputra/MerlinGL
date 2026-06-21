project(MerlinEngine)

find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

add_executable(MerlinEngine
        src/main.cpp
        src/utils.h
        src/Core/Application.cpp
        src/Core/Application.h
        src/GUI/GuiModule.cpp
        src/GUI/GuiModule.h
        src/Core/Window.cpp
        src/Core/Window.h
        src/Engine/Framebuffer.cpp
        src/Engine/Framebuffer.h
        src/Engine/Renderer.cpp
        src/Engine/Renderer.h
        src/Engine/Camera.cpp
        src/Engine/Camera.h
        src/Engine/Shader.cpp
        src/Engine/Shader.h
)

target_link_libraries(MerlinEngine PRIVATE
        spdlog::spdlog
        assimp::assimp
        glfw
        glad::glad
        glm::glm
        imgui::imgui
)

target_include_directories(MerlinEngine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_precompile_headers(MerlinEngine PRIVATE src/pch.h)

add_custom_command(TARGET MerlinEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/src/Engine/shaders"
        "$<TARGET_FILE_DIR:MerlinEngine>/shaders"
)
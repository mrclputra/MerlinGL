project(Merlin)

find_package(spdlog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(EnTT CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_image.h")

# This is the engine library
# todo: maybe figure out dynamic linking?
add_library(Merlin STATIC
        src/Window.cpp
        src/Window.h
        src/Framebuffer.cpp
        src/Framebuffer.h
        src/Renderer.cpp
        src/Renderer.h
        src/Camera.cpp
        src/Camera.h
        src/components/Shader.cpp
        src/components/Shader.h
        src/Scene.h
        src/components/Transform.h
        src/Events.h
        src/EventBus.h
        src/lights/DirectionalLight.h
        src/Loader.cpp
        src/Loader.h
        src/components/Mesh.h
        src/components/Material.h
        src/components/Texture.h
        src/components/PointCloud.h
)

target_link_libraries(Merlin PUBLIC
        spdlog::spdlog
        assimp::assimp
        glfw
        glad::glad
        glm::glm
        EnTT::EnTT
)

target_include_directories(Merlin PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${STB_INCLUDE_DIRS}
)

target_precompile_headers(Merlin PRIVATE src/core_pch.h)

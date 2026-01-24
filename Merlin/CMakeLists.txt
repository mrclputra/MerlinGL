project(MerlinEngine)

set(EXT_DIR "${CMAKE_SOURCE_DIR}/external")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# collect sources
file(GLOB_RECURSE ENGINE_SOURCES "*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "*.h")
file(GLOB IMGUI_SOURCES "${EXT_DIR}/imgui/*.cpp")
set(IMGUI_BACKENDS 
    "${EXT_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${EXT_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)
set(GLAD_SOURCE "${EXT_DIR}/glad/src/glad.c")

add_subdirectory("${EXT_DIR}/glfw" "${CMAKE_CURRENT_BINARY_DIR}/glfw")
add_subdirectory("${EXT_DIR}/assimp" "${CMAKE_CURRENT_BINARY_DIR}/assimp")
add_subdirectory("${EXT_DIR}/ImGuiFileDialog" "${CMAKE_CURRENT_BINARY_DIR}/ImGuiFileDialog")

# library
add_library(MerlinEngine SHARED 
    ${ENGINE_SOURCES} ${ENGINE_HEADERS} 
    ${IMGUI_SOURCES} ${IMGUI_BACKENDS}
    ${GLAD_SOURCE}
)

target_include_directories(MerlinEngine PUBLIC 
    "${CMAKE_SOURCE_DIR}"
    "${EXT_DIR}/glm"
    "${EXT_DIR}/glad/include"
    "${EXT_DIR}/imgui"
    "${EXT_DIR}/imgui/backends"
)
target_include_directories(MerlinEngine PRIVATE src)
target_include_directories(ImGuiFileDialog PUBLIC "${EXT_DIR}/imgui")

target_link_libraries(MerlinEngine PUBLIC glfw assimp::assimp ImGuiFileDialog)
target_compile_definitions(MerlinEngine PRIVATE MERLIN_EXPORT_DLL IMGUI_IMPL_OPENGL_LOADER_CUSTOM)

target_precompile_headers(MerlinEngine PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:Merlin/mepch.h>"
)
project(MerlinEngine)

set(EXT_DIR "${CMAKE_SOURCE_DIR}/external")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

# just the stuff we need for imgui
set(IMGUI_BACKENDS 
    "${EXT_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${EXT_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)
file(GLOB IMGUI_SOURCES "${EXT_DIR}/imgui/*.cpp")
file(GLOB DIALOG_SOURCES "${EXT_DIR}/ImGuiFileDialog/*.cpp")

file(GLOB_RECURSE ENGINE_SOURCES "*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "*.h")

add_subdirectory("${EXT_DIR}/glfw" "${CMAKE_CURRENT_BINARY_DIR}/glfw")
add_subdirectory("${EXT_DIR}/assimp" "${CMAKE_CURRENT_BINARY_DIR}/assimp")

# library
add_library(MerlinEngine SHARED 
    ${ENGINE_SOURCES} ${ENGINE_HEADERS} 
    ${IMGUI_SOURCES} ${IMGUI_BACKENDS} ${DIALOG_SOURCES}
)

target_include_directories(MerlinEngine PUBLIC 
    "${CMAKE_SOURCE_DIR}"
    "${EXT_DIR}/glm"
    "${EXT_DIR}/glad/include"
    "${EXT_DIR}/imgui"
    "${EXT_DIR}/imgui/backends"
    "${EXT_DIR}/ImGuiFileDialog"
)
target_include_directories(MerlinEngine PRIVATE src)

target_link_libraries(MerlinEngine PUBLIC glfw assimp::assimp)
target_compile_definitions(MerlinEngine PRIVATE MERLIN_EXPORT_DLL)
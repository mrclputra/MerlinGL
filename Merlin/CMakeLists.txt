project(MerlinEngine)

set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# collect sources
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/window/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/window/*.h")
file(GLOB IMGUI_SOURCES "${EXT_DIR}/imgui/*.cpp")
set(IMGUI_BACKENDS 
    "${EXT_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${EXT_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)
set(GLAD_SOURCE "${EXT_DIR}/glad/src/glad.c")

set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("${EXT_DIR}/glfw" EXCLUDE_FROM_ALL)
add_subdirectory("${EXT_DIR}/assimp" EXCLUDE_FROM_ALL)
add_subdirectory("${EXT_DIR}/imguifiledialog" EXCLUDE_FROM_ALL)

# library
add_library(MerlinEngine SHARED 
    ${ENGINE_SOURCES} 
    ${ENGINE_HEADERS} 
    ${IMGUI_SOURCES} 
    ${IMGUI_BACKENDS}
    ${GLAD_SOURCE}
)

target_include_directories(MerlinEngine 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${EXT_DIR}/glm"
        "${EXT_DIR}/glad/include"
        "${EXT_DIR}/imgui"
    PRIVATE
        "${EXT_DIR}/imgui/backends"
)

target_include_directories(ImGuiFileDialog PUBLIC "${EXT_DIR}/imgui")

# linking
target_link_libraries(MerlinEngine PUBLIC glfw assimp::assimp ImGuiFileDialog)

# compile config
target_compile_definitions(MerlinEngine PRIVATE "MERLIN_API=__declspec(dllexport)")
target_compile_definitions(MerlinEngine INTERFACE "MERLIN_API=__declspec(dllimport)")
target_compile_definitions(MerlinEngine PRIVATE "IMGUI_API=__declspec(dllexport)")
target_compile_definitions(MerlinEngine INTERFACE "IMGUI_API=__declspec(dllimport)")

target_precompile_headers(MerlinEngine PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/mepch.h>"
)
project(MerlinEngine)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp" "${EXT_DIR}/imgui/*.cpp" "${EXT_DIR}/ImGuiFileDialog/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "include/*.h" "src/*.h")
set(EXT_DIR "${CMAKE_SOURCE_DIR}/external")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)

add_subdirectory("${EXT_DIR}/glfw" "${CMAKE_CURRENT_BINARY_DIR}/glfw")
add_subdirectory("${EXT_DIR}/assimp" "${CMAKE_CURRENT_BINARY_DIR}/assimp")

# library
add_library(MerlinEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(MerlinEngine PUBLIC 
	include
    "${EXT_DIR}/glm"
    "${EXT_DIR}/glad/include"
    "${EXT_DIR}/imgui"
    "${EXT_DIR}/ImGuiFileDialog"
)
target_include_directories(MerlinEngine PRIVATE src)

target_link_libraries(MerlinEngine PUBLIC glfw assimp::assimp)
target_compile_definitions(MerlinEngine PRIVATE MERLIN_EXPORT_DLL)
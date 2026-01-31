project(MerlinEngine)

set(EXT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external")

# collect sources
file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS "src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/window/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS CONFIGURE_DEPENDS "src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/window/*.h")
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
    "${EXT_DIR}/stb_image.cpp"
)

# generate export header,
# for cross-platform symbol visibility
include(GenerateExportHeader)
generate_export_header(MerlinEngine
    BASE_NAME MERLIN
    EXPORT_MACRO_NAME MERLIN_API
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/src/merlin_export.h"
)

target_include_directories(MerlinEngine 
    PUBLIC 
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${EXT_DIR}/glm"
        "${EXT_DIR}/glad/include"
        "${EXT_DIR}/imgui"
        "${EXT_DIR}"   
    PRIVATE
        "${EXT_DIR}/imgui/backends"
)

target_include_directories(ImGuiFileDialog PUBLIC "${EXT_DIR}/imgui")

# linking
target_link_libraries(MerlinEngine PUBLIC glfw assimp::assimp ImGuiFileDialog)

# ImGui is compiled into MerlinEngine, 
# we need to ensure its symbols are visible
if(WIN32)
    # On Windows, export ImGui symbols
    target_compile_definitions(MerlinEngine PRIVATE "IMGUI_API=__declspec(dllexport)")
    target_compile_definitions(MerlinEngine INTERFACE "IMGUI_API=__declspec(dllimport)")
else()
    # use visibility attributes instead
    target_compile_definitions(MerlinEngine PUBLIC "IMGUI_API=__attribute__((visibility(\"default\")))")
    
    # Set default visibility to hidden, but ImGui symbols will be visible due to IMGUI_API
    set_target_properties(MerlinEngine PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
    )
endif()

target_compile_definitions(MerlinEngine PRIVATE 
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM  # disable imgui's loader
)

target_precompile_headers(MerlinEngine PRIVATE 
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_BINARY_DIR}/src/merlin_export.h>"
    "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/src/mepch.h>"
)
project(MerlinEngine)

file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
file(GLOB_RECURSE ENGINE_HEADERS "include/*.h" "src/*.h")

# library
# todo: consider using a dll instead
add_library(MerlinEngine SHARED ${ENGINE_SOURCES} ${ENGINE_HEADERS})

target_include_directories(MerlinEngine PUBLIC include)
target_include_directories(MerlinEngine PRIVATE src)
target_compile_definitions(MerlinEngine PRIVATE MERLIN_EXPORT_DLL)
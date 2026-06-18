#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "utils.h"
#include "Core/Application.h"

int main() {
    // configure logger
    spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [%s:%#] %v");
    SPDLOG_INFO("logger configured");
    SPDLOG_INFO("spdlog v{}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    SPDLOG_INFO("hello MerlinGL!!");
    SPDLOG_INFO("platform: {}", utils::getPlatformName());

    // some notes
    // - this is a rewrite of the engine, use ../.src/ for old system reference
    // - I can turn it into a library later down the line once everything is in place

    auto app = Merlin::Application();
    app.run();

    return 0;
}

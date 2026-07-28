#include "Application.h"
#include "Window.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ringbuffer_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/async.h>

constexpr const char *getPlatformName() {
#if defined(_WIN32)
   return "Windows";
#elif defined(__APPLE__) && defined(__MACH__)
   return "macOS";
#elif defined(__linux__)
   return "Linux";
#elif defined(__FreeBSD__)
   return "FreeBSD";
#else
   return "Unknown Platform";
#endif
}

int main() {
   // configure logger
   //    - use mt or st for multi or single threaded behavior
   auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();  // std output
   auto rbs_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(20);
   // auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log", 1048576 * 5, 1); // warning: sinks only write

   spdlog::init_thread_pool(8192, 1);
   auto logger = std::make_shared<spdlog::async_logger>("main", spdlog::sinks_init_list{stdout_sink, rbs_sink}, spdlog::thread_pool(), spdlog::async_overflow_policy::overrun_oldest);
   spdlog::set_default_logger(logger);
   spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [%s:%#] %v");
   SPDLOG_INFO("logger configured");
   SPDLOG_INFO("spdlog v{}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

   SPDLOG_INFO("hello MerlinGL!!");
   SPDLOG_INFO("platform: {}", getPlatformName());

   auto window = Merlin::Window("Merlin", 1280, 720);
   auto app = Merlin::Application(window);
   app.run();

   window.Shutdown();
   glfwTerminate();

   return 0;
}
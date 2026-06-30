#include "pch.h"
#include "Core/Application.h"
#include "utils.h"

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
   SPDLOG_INFO("platform: {}", utils::getPlatformName());

   // some notes
   // - this is a rewrite of the rendering engine; use ../.src/ for old system reference
   // - I should turn it into a library later down the line once everything is in the right place

   auto app = Merlin::Application("Merlin", 1280, 720);
   app.run();

   return 0;
}
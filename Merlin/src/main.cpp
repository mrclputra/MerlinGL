#include "Core/Application.h"
#include "pch.h"
#include "utils.h"

int main() {
   // configure logger
   //    - use mt or st for multi or singlethreaded behavior
   auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();  // std output
   auto rbs_sink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(20);
   // auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("log", 1048576 * 5, 1); // warning: sinks only write

   auto logger = std::make_shared<spdlog::logger>("main", spdlog::sinks_init_list{stdout_sink, rbs_sink});  // add sinks to the logger
   spdlog::set_default_logger(logger);
   spdlog::set_pattern("[%H:%M:%S] [%^%l%$] [%s:%#] %v");
   SPDLOG_INFO("logger configured");
   SPDLOG_INFO("spdlog v{}.{}.{}", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

   SPDLOG_INFO("hello MerlinGL!!");
   SPDLOG_INFO("platform: {}", utils::getPlatformName());

   // some notes
   // - this is a rewrite of the engine, use ../.src/ for old system reference
   // - I can turn it into a library later down the line once everything is in the right place

   auto app = Merlin::Application("MerlinGL", 1280, 720);
   app.run();

   return 0;
}
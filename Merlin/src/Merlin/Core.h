#pragma once

#ifdef _WIN32
#ifdef MERLIN_EXPORT_DLL
#define MERLIN_API __declspec(dllexport)
#else
#define MERLIN_API __declspec(dllimport)
#endif
#else
#define MERLIN_API
#endif

#ifdef ME_DEBUG
#define ME_ENABLE_ASSERTS
#endif

#ifdef ME_ENABLE_ASSERTS
#define ME_ASSERT(x, ...) { if(!(x)) { logger.error("Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
#define ME_CORE_ASSERT(x, ...) { if(!(x)) { logger.error("Core Assertion Failed: ", __VA_ARGS__); __debugbreak(); } }
#else
#define ME_ASSERT(x, ...)
#define ME_CORE_ASSERT(x, ...)
#endif
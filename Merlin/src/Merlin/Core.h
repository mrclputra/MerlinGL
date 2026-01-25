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

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

// todo: proper debug mode assertions
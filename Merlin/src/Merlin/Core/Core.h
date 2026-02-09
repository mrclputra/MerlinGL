#pragma once

#ifdef _WIN32
	#ifdef MERLIN_BUILD_DLL
		#define MERLIN_API __declspec(dllexport)
	#elif defined(__INTELLISENSE__)
		// special case cuz intellisense is stupid
		#define MERLIN_API
	#else
		#define MERLIN_API __declspec(dllimport)
	#endif
#else
	#define MERLIN_API
#endif

#define BIT(x) (1 << x)

// bind a member function to this instance for event callbacks
#define MERLIN_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { \
	return this->fn(std::forward<decltype(args)>(args)...); \
}

// todo: proper debug mode assertions
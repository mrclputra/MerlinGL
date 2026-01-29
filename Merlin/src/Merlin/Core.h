#pragma once

#include "src/merlin_export.h"

#define BIT(x) (1 << x)

// bind a member function to this instance for event callbacks
#define MERLIN_BIND_FN(fn) [this](auto&&... args) -> decltype(auto) { \
	return this->fn(std::forward<decltype(args)>(args)...); \
}

// todo: proper debug mode assertions
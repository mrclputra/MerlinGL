#pragma once

#include "src/merlin_export.h"

#define BIT(x) (1 << x)
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

// todo: proper debug mode assertions
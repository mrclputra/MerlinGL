#pragma once

#include <Merlin/Core.h>

// we may not end up needing a template class
// as imgui may already implement this kind of functionality for us? will need to read into this
namespace Merlin {
	class MERLIN_API Widget {
	public:
		virtual ~Widget() = default;
		virtual void OnImGuiRender() = 0;

		bool IsVisible = true;
	};
}
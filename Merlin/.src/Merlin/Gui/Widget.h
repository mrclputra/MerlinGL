#pragma once

namespace Merlin {
	class Widget {
	public:
		virtual ~Widget() = default;
		virtual void Render() = 0;

		bool IsVisible = true;
	};
}
#pragma once

#include <memory>

namespace BuD
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Show() = 0;
		virtual void ProcessEvents() = 0;

		virtual int Width() = 0;
		virtual int Height() = 0;

		virtual void ToggleFullscreen() = 0;

	protected:
		bool m_fullscreen = false;
	};
}
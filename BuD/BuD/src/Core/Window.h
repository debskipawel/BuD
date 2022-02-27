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

		inline int Width() { return m_width; }
		inline int Height() { return m_height; }

	protected:
		int m_width = 800, m_height = 600;
		bool m_fullscreen = false;
	};
}
#pragma once

namespace BuD
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Show() = 0;
		virtual bool EventQueueNotEmpty() = 0;
		virtual void HandleEvent() = 0;

	protected:
		int m_width = 800, m_height = 600;
		bool m_fullscreen = false;
	};
}
#pragma once

#include <memory>

#include "Renderer.h"

namespace BuD
{
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void Show() = 0;
		virtual void ProcessEvents() = 0;

		inline void Draw() { m_renderer->Draw(); }
		inline void Resize(int width, int height)
		{
			m_width = width;
			m_height = height;

			m_renderer->UpdateBufferSize(width, height);
		}

		inline int Width() { return m_width; }
		inline int Height() { return m_height; }

	protected:
		int m_width = 800, m_height = 600;
		bool m_fullscreen = false;

		std::shared_ptr<BuD::Renderer> m_renderer;
	};
}
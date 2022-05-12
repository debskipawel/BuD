#pragma once

#include "DirectX11/DX11Renderer.h"
#include "Win32/Win32Window.h"

namespace BuD
{
	class GuiLayer
	{
	public:
		GuiLayer(std::shared_ptr<DX11Renderer> renderer, std::shared_ptr<Win32Window> window);
		~GuiLayer();

		void BeginFrame();
		void EndFrame();

		void UpdateRenderer(std::shared_ptr<DX11Renderer> renderer)
		{
			m_renderer = renderer;
		}

	private:
		std::shared_ptr<DX11Renderer> m_renderer;
	};
}

#pragma once

#include "DirectX11/DX11Device.h"
#include "Win32/Win32Window.h"

namespace BuD
{
	class GuiLayer
	{
	public:
		GuiLayer(const DX11Device& device, std::shared_ptr<Win32Window> window);
		~GuiLayer();

		void BeginFrame();
		void EndFrame();
	};
}

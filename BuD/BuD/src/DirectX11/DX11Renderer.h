#pragma once

#ifdef _WIN32

#include "Core/Renderer.h"
#include "Win32/Win32Window.h"

#include <Windows.h>
#include <d3d11_1.h>
#include <memory>

namespace BuD
{
	class DX11Renderer : public Renderer
	{
	public:
		virtual void UpdateBufferSize(int width, int height) override;
		virtual void Draw() override;

		DX11Renderer(std::shared_ptr<Win32Window> window);
		~DX11Renderer();

	private:

		void Cleanup();

		IDXGISwapChain* m_swapChain;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_deviceContext;
		ID3D11RenderTargetView* m_rtv;

		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		int colormodr = 1;
		int colormodg = 1;
		int colormodb = 1;
	};
}

#endif

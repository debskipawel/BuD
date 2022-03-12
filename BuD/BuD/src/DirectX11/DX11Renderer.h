#pragma once

#ifdef _WIN32

#include "../Core/Renderer.h"
#include "../Win32/Win32Window.h"

#include "DX11Device.h"

#include <Windows.h>
#include <d3d11_1.h>
#include <memory>

namespace BuD
{
	class DX11Renderer : public Renderer
	{
	public:
		inline const DX11Device& Device() const { return m_device; }

		virtual void UpdateBufferSize(int width, int height) override;
		
		virtual void Begin() override;
		virtual void Draw(const RenderableSceneEntity& entity) override;
		virtual void End() override;

		DX11Renderer(std::shared_ptr<Win32Window> window);
		~DX11Renderer();

	private:

		void Cleanup();

		DX11Device m_device;
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

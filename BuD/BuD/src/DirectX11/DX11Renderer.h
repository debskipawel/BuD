#pragma once

#ifdef _WIN32

#include "Core/Renderer.h"
#include "Win32/Win32Window.h"

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

		virtual void UpdateBuffersSize(int width, int height) override;

		virtual void Begin() override;
		virtual void Draw(std::shared_ptr<RenderableSceneEntity> entity, std::shared_ptr<AbstractCamera> camera) override;
		virtual void End() override;

		DX11Renderer(std::shared_ptr<Win32Window> window);

	private:
		DX11Device m_device;
		ComPtr<ID3D11RenderTargetView> m_backBuffer;
		ComPtr<ID3D11DepthStencilView> m_depthBuffer;
	};
}

#endif

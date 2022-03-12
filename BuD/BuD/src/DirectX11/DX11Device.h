#pragma once

#include <d3d11.h>
#include <wrl/client.h>

#include "../Win32/Win32Window.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11Device
	{
	public:
		explicit DX11Device(std::shared_ptr<Win32Window> window);

		inline ID3D11Device* Raw() const { return m_device.Get(); }
		inline ID3D11Device* operator->() const { return Raw(); }
		inline const ComPtr<ID3D11DeviceContext>& Context() const { return m_context; }
		inline const ComPtr<IDXGISwapChain>& SwapChain() const { return m_swapchain; }

		ComPtr<ID3D11RenderTargetView> CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture);
		ComPtr<ID3D11DepthStencilView> CreateDepthStencilBuffer(UINT width, UINT height);
		ComPtr<ID3D11Texture2D> CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const;
		
	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
		ComPtr<IDXGISwapChain> m_swapchain;
	};
}

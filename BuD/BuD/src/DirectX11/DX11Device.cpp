#include "DX11Device.h"

#include "DX11Structures.h"

namespace BuD
{
	DX11Device::DX11Device(std::shared_ptr<Win32Window> window)
	{
		DX11SwapchainDesc swapChainDesc(window->Handle(), window->Width(), window->Height());

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		IDXGISwapChain* swapchain = nullptr;

		auto hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
			D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, nullptr, &context);

		m_device.Attach(device);
		m_swapchain.Attach(swapchain);
		m_context.Attach(context);
	}

	ComPtr<ID3D11RenderTargetView> DX11Device::CreateRenderTargetView(const ComPtr<ID3D11Texture2D>& texture)
	{
		ID3D11RenderTargetView* temp;
		auto hr = m_device->CreateRenderTargetView(texture.Get(), nullptr, &temp);
		ComPtr<ID3D11RenderTargetView> result(temp);
		
		if (FAILED(hr))
		{
			printf("Creating rtv failed\n");
		}

		return result;
	}

	ComPtr<ID3D11DepthStencilView> DX11Device::CreateDepthStencilBuffer(UINT width, UINT height)
	{
		auto desc = DX11Texture2DDesc::DepthStencilDescription(width, height);
		auto tex = CreateTexture(desc);

		ID3D11DepthStencilView* temp;
		auto hr = m_device->CreateDepthStencilView(tex.Get(), nullptr, &temp);
		ComPtr<ID3D11DepthStencilView> result(temp);
		
		if (FAILED(hr))
		{
			printf("Error while creating depth buffer\n");
		}

		return result;
	}
	ComPtr<ID3D11Texture2D> DX11Device::CreateTexture(const D3D11_TEXTURE2D_DESC& desc) const
	{
		ID3D11Texture2D* temp;
		auto hr = m_device->CreateTexture2D(&desc, nullptr, &temp);
		ComPtr<ID3D11Texture2D> result(temp);
		
		if (FAILED(hr))
		{
			printf("Error while creating a texture\n");
		}
		
		return result;
	}
}
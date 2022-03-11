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

		auto hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
			D3D11_SDK_VERSION, &swapChainDesc, &swapchain, &device, NULL, &context);

		m_device.Attach(device);
		m_swapchain.Attach(swapchain);
		m_context.Attach(context);
	}
}
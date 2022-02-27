#ifdef _WIN32

#include "DX11Renderer.h"

BuD::DX11Renderer::DX11Renderer(std::shared_ptr<Win32Window> window)
{
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = window->Width();
	bufferDesc.Height = window->Height();
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window->m_hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
		D3D11_SDK_VERSION, &swapChainDesc, &m_swapChain, &m_device, NULL, &m_deviceContext);

	ID3D11Texture2D* backBuffer;

	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	m_device->CreateRenderTargetView(backBuffer, NULL, &m_rtv);

	backBuffer->Release();

	m_deviceContext->OMSetRenderTargets(1, &m_rtv, NULL);
}

BuD::DX11Renderer::~DX11Renderer()
{
	Cleanup();
}

void BuD::DX11Renderer::Cleanup()
{
	if (m_device)
	{
		m_device->Release();
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
	}

	if (m_rtv)
	{
		m_rtv->Release();
	}
}

void BuD::DX11Renderer::UpdateBufferSize(int width, int height)
{
	m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
}

void BuD::DX11Renderer::Draw()
{
	red += colormodr * 0.00005f;
	green += colormodg * 0.00002f;
	blue += colormodb * 0.00001f;

	if (red >= 1.0f || red <= 0.0f)
		colormodr *= -1;
	if (green >= 1.0f || green <= 0.0f)
		colormodg *= -1;
	if (blue >= 1.0f || blue <= 0.0f)
		colormodb *= -1;

	FLOAT color[] = { red, green, blue, 1.0f };

	m_deviceContext->ClearRenderTargetView(m_rtv, color);

	m_swapChain->Present(0, 0);
}

#endif

#include "DX11Structures.h"

namespace BuD
{
	DX11SwapchainDesc::DX11SwapchainDesc(HWND wndHwnd, UINT width, UINT height)
	{
		ZeroMemory(this, sizeof(DX11SwapchainDesc));
		BufferDesc.Width = width;
		BufferDesc.Height = height;
		BufferDesc.RefreshRate.Numerator = 120;
		BufferDesc.RefreshRate.Denominator = 1;
		BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //0
		//BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //0
		SampleDesc.Quality = 0;
		SampleDesc.Count = 1;
		BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		BufferCount = 1;
		OutputWindow = wndHwnd;
		Windowed = true;
		//SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //0
		//Flags = 0;
	}
	
	DX11BufferDesc::DX11BufferDesc(UINT bindFlags, size_t byteWidth)
	{
		ZeroMemory(this, sizeof(DX11BufferDesc));
		BindFlags = bindFlags;
		ByteWidth = static_cast<UINT>(byteWidth);
		Usage = D3D11_USAGE_DEFAULT;
		//CPUAccessFlags = 0;
		//MiscFlags = 0;
		//StructureByteStride = 0;
	}
}

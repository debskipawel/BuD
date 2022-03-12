#pragma once

#include <d3d11.h>

namespace BuD
{
	struct DX11SwapchainDesc : public DXGI_SWAP_CHAIN_DESC
	{
		DX11SwapchainDesc(HWND wndHwnd, UINT width, UINT height);
	};

	struct DX11Texture2DDesc : D3D11_TEXTURE2D_DESC
	{
		DX11Texture2DDesc(UINT width, UINT height);

		static DX11Texture2DDesc DepthStencilDescription(UINT width, UINT height);
	};

	struct DX11Viewport : D3D11_VIEWPORT
	{
		explicit DX11Viewport(SIZE size);
	};

	struct DX11BufferDesc : D3D11_BUFFER_DESC
	{
		DX11BufferDesc(UINT bindFlags, size_t byteWidth);

		inline static DX11BufferDesc VertexBufferDescription(size_t byteWidth)
		{
			return { D3D11_BIND_VERTEX_BUFFER, byteWidth };
		}

		inline static DX11BufferDesc IndexBufferDescription(size_t byteWidth)
		{
			return { D3D11_BIND_INDEX_BUFFER, byteWidth };
		}

		inline static DX11BufferDesc ConstantBufferDescription(size_t byteWidth)
		{
			DX11BufferDesc desc{ D3D11_BIND_CONSTANT_BUFFER, byteWidth };
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			return desc;
		}
	};
}

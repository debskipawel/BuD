#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "DirectX11/DX11Structures.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11Buffer
	{
	public:
		DX11Buffer(ID3D11Device* device, size_t size, const void* data, DX11BufferDesc desc);
		virtual ~DX11Buffer() = default;

		inline ID3D11Buffer* Buffer() const { return m_buffer.Get(); }

		virtual void Update(ID3D11DeviceContext* context, const void* data, size_t size);

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const = 0;

		size_t m_bufferSize;
		ComPtr<ID3D11Buffer> m_buffer;
	};

	size_t BitsPerPixel(DXGI_FORMAT fmt);
}
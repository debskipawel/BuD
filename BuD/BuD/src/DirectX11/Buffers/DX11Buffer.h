#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "../DX11Structures.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11Buffer
	{
	public:
		explicit DX11Buffer(ID3D11Device* device, size_t size, const void* data = nullptr);

		inline ID3D11Buffer* Buffer() { return m_buffer.Get(); }

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) = 0;

		size_t m_bufferSize;
		ComPtr<ID3D11Buffer> m_buffer;
	};
}

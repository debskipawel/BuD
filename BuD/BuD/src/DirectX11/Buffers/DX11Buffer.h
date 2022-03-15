#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <vector>

#include "DirectX11/DX11Structures.h"
#include "DirectX11/DX11Device.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11Buffer
	{
	public:
		DX11Buffer(const DX11Device& device, const void* data, DX11BufferDesc desc);
		virtual ~DX11Buffer() = default;

		inline ID3D11Buffer* Buffer() const { return m_buffer.Get(); }

		virtual void Update(const void* data, size_t size);

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const = 0;

		virtual void CreateBuffer(const void* data, const DX11BufferDesc& desc);

		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;

		DX11BufferDesc m_desc;
		ComPtr<ID3D11Buffer> m_buffer;
	};

	size_t BitsPerPixel(DXGI_FORMAT fmt);
}

#pragma once

#include "DX11Buffer.h"

namespace BuD
{
	class DX11IndexBuffer : public DX11Buffer
	{
	public:
		explicit DX11IndexBuffer(ID3D11Device* device, DXGI_FORMAT format, size_t size, const void* data = nullptr);

		inline UINT Count() const { return m_indicesCount; }
		inline DXGI_FORMAT Format() const { return m_format; }

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const override;

		DXGI_FORMAT m_format;
		UINT m_indicesCount;
	};
}
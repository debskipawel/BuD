#pragma once

#include "DX11Buffer.h"

namespace BuD
{
	class DX11VertexBuffer : public DX11Buffer
	{
	public:
		explicit DX11VertexBuffer(ID3D11Device* device, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data = nullptr);

		inline UINT Stride() const { return m_stride; }
		inline UINT Offset() const { return m_offset; }

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const override;

		size_t m_stride;
		size_t m_offset;
	};
}

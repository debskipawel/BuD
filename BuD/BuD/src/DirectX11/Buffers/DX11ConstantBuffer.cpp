#include "DX11ConstantBuffer.h"

namespace BuD
{
	DX11BufferDesc DX11ConstantBuffer::GetBufferDesc(size_t byteWidth)
	{
		auto desc = DX11BufferDesc{ D3D11_BIND_CONSTANT_BUFFER, byteWidth };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return desc;
	}

	void DX11ConstantBuffer::UpdateData(ID3D11DeviceContext* context, const void* data, size_t size)
	{
		if (size == m_bufferSize)
		{
			D3D11_MAPPED_SUBRESOURCE res;
			
			context->Map(m_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
			memcpy(res.pData, &data, size);
			context->Unmap(m_buffer.Get(), 0);
		}
	}
}

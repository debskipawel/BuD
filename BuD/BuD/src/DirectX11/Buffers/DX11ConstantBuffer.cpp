#include "DX11ConstantBuffer.h"

namespace BuD
{
	DX11BufferDesc DX11ConstantBuffer::GetBufferDesc(size_t byteWidth) const
	{
		auto desc = DX11BufferDesc{ D3D11_BIND_CONSTANT_BUFFER, byteWidth };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		return desc;
	}
}

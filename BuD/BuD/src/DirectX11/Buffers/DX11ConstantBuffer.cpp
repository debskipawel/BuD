#include "DX11ConstantBuffer.h"

namespace BuD
{
	DX11BufferDesc DX11ConstantBuffer::GetBufferDesc(size_t byteWidth) const
	{
		return DX11BufferDesc::ConstantBufferDescription(byteWidth);
	}
}

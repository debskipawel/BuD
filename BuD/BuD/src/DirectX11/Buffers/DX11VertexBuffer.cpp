#include "DX11VertexBuffer.h"

namespace BuD
{
	DX11BufferDesc DX11VertexBuffer::GetBufferDesc(size_t byteWidth)
	{
		return DX11BufferDesc{ D3D11_BIND_VERTEX_BUFFER, byteWidth };
	}
}
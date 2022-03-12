#include "DX11IndexBuffer.h"

namespace BuD
{
	DX11IndexBuffer::DX11IndexBuffer(ID3D11Device* device, DXGI_FORMAT format, size_t size, const void* data)
		: DX11Buffer(device, data, GetBufferDesc(size)), m_format(format)
	{
        auto bpp = BitsPerPixel(format);
        m_indicesCount = size / (bpp / 8);
	}

    DX11BufferDesc DX11IndexBuffer::GetBufferDesc(size_t byteWidth) const
    {
        return DX11BufferDesc { D3D11_BIND_INDEX_BUFFER, byteWidth };
    }
}
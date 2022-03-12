#include "DX11VertexBuffer.h"

#include <numeric>
#include <SimpleMath.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
    struct PositionNormal
    {
        Vector3 position, normal;
    };

	DX11VertexBuffer::DX11VertexBuffer(ID3D11Device* device, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const void* data)
		: DX11Buffer(device, data, GetBufferDesc(size)), m_offset(0)
	{
		auto strideBit = std::accumulate(layout.begin(), layout.end(), (size_t)0, [](size_t a, D3D11_INPUT_ELEMENT_DESC desc) { return a + BitsPerPixel(desc.Format); });
		m_stride = strideBit / 8;
	}

    DX11BufferDesc DX11VertexBuffer::GetBufferDesc(size_t byteWidth) const
	{
		return DX11BufferDesc{ D3D11_BIND_VERTEX_BUFFER, byteWidth };
	}
}
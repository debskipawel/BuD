#include "DX11Buffer.h"

#include "../DX11Structures.h"

namespace BuD
{
	DX11Buffer::DX11Buffer(ID3D11Device* device, size_t size, const void* data)
		: m_bufferSize(size)
	{
		auto desc = this->GetBufferDesc(size);
		
		D3D11_SUBRESOURCE_DATA sdata;
		ZeroMemory(&sdata, sizeof sdata);
		sdata.pSysMem = data;

		ID3D11Buffer* temp;
		auto hr = device->CreateBuffer(&desc, data ? &sdata : nullptr, &temp);

		m_buffer.Attach(temp);
	}
}
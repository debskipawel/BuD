#pragma once

#include "DX11Buffer.h"

namespace BuD
{
	class DX11ConstantBuffer : public DX11Buffer
	{
	public:
		explicit DX11ConstantBuffer(ID3D11Device* device, size_t size, const void* data = nullptr)
			: DX11Buffer(device, size, data, GetBufferDesc(size)) {}

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const override;
	};
}
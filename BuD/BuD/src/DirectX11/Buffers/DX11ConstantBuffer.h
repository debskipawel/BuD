#pragma once

#include "DX11Buffer.h"

namespace BuD
{
	class DX11ConstantBuffer : public DX11Buffer
	{
	public:
		explicit DX11ConstantBuffer(const DX11Device& device, size_t size, const void* data = nullptr)
			: DX11Buffer(device, data, GetBufferDesc(size)) {}

	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) const override;
	};
}
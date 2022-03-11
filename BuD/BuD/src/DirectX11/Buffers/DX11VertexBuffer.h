#pragma once

#include "DX11Buffer.h"

namespace BuD
{
	class DX11VertexBuffer : public DX11Buffer
	{
	protected:
		virtual DX11BufferDesc GetBufferDesc(size_t byteWidth) override;
	};
}

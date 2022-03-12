#pragma once

#include <vector>
#include <memory>

#include "../Buffers/DX11ConstantBuffer.h"

namespace BuD
{
	class DX11Shader
	{
	public:
		inline void UpdateConstantBuffer(ID3D11DeviceContext* context, UINT bufferIndex, const void* data, size_t size)
		{
			m_constantBuffers[bufferIndex]->Update(context, data, size);
		}

		inline const auto& ConstantBuffers() const { return m_constantBuffers; }
		ID3D11Buffer** RawConstantBuffers();

	protected:
		std::vector<std::shared_ptr<DX11ConstantBuffer>> m_constantBuffers;
		std::vector<ID3D11Buffer*> m_rawConstantBuffers;
	};
}

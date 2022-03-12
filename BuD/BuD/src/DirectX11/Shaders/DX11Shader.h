#pragma once

#include <vector>
#include <memory>

#include "../Buffers/DX11ConstantBuffer.h"

namespace BuD
{
	class DX11Shader
	{
	public:
		inline const auto& ConstantBuffers() const { return m_constantBuffers; }
		ID3D11Buffer** RawConstantBuffers();

	protected:
		std::vector<std::shared_ptr<DX11ConstantBuffer>> m_constantBuffers;
		std::vector<ID3D11Buffer*> m_rawConstantBuffers;
	};
}

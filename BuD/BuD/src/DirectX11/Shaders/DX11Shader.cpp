#include "DX11Shader.h"

#include <algorithm>

namespace BuD
{
	ID3D11Buffer** DX11Shader::RawConstantBuffers()
	{
		m_rawConstantBuffers.resize(m_constantBuffers.size());

		std::transform(
			m_constantBuffers.begin(), m_constantBuffers.end(), 
			m_rawConstantBuffers.begin(), 
			[](std::shared_ptr<DX11ConstantBuffer> b) { return b->Buffer(); }
		);

		return m_rawConstantBuffers.data();
	}
}

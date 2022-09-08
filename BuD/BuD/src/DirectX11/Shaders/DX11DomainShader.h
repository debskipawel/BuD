#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

#include "DX11Shader.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11DomainShader : public DX11Shader
	{
	public:
		explicit DX11DomainShader(const DX11Device& device, void* code, size_t size);

		inline ID3D11DomainShader* Shader() { return m_shader.Get(); }
	private:
		ComPtr<ID3D11DomainShader> m_shader;
	};
}
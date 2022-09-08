#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

#include "DX11Shader.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11HullShader : public DX11Shader
	{
	public:
		explicit DX11HullShader(const DX11Device& device, void* code, size_t size);

		inline ID3D11HullShader* Shader() { return m_shader.Get(); }
	private:
		ComPtr<ID3D11HullShader> m_shader;
	};
}
#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

#include "DX11Shader.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11PixelShader : public DX11Shader
	{
	public:
		explicit DX11PixelShader(const DX11Device& device, void* code, size_t size);

		inline ID3D11PixelShader* Shader() { return m_shader.Get(); }
	private:
		ComPtr<ID3D11PixelShader> m_shader;
	};
}
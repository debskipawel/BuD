#pragma once

#include "DX11Shader.h"

namespace BuD
{
	class DX11GeometryShader : public DX11Shader
	{
	public:
		explicit DX11GeometryShader(const DX11Device& device, void* code, size_t size);

		inline ID3D11GeometryShader* Shader() { return m_shader.Get(); }
	private:
		ComPtr<ID3D11GeometryShader> m_shader;
	};
}

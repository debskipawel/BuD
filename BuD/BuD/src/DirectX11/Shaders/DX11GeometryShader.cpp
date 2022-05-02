#include "DX11GeometryShader.h"

namespace BuD
{
	DX11GeometryShader::DX11GeometryShader(const DX11Device& device, void* code, size_t size)
	{
		device->CreateGeometryShader(code, size, nullptr, m_shader.GetAddressOf());
	}
}

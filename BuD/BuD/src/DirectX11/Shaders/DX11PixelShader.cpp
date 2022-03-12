#include "DX11PixelShader.h"

namespace BuD
{
	DX11PixelShader::DX11PixelShader(ID3D11Device* device, void* code, size_t size)
	{
		ID3D11PixelShader* shader = nullptr;

		device->CreatePixelShader(code, size, nullptr, &shader);
		m_shader.Attach(shader);
	}
}
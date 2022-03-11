#include "DX11PixelShader.h"

namespace BuD
{
	DX11PixelShader::DX11PixelShader(ID3D11Device* device, const std::vector<BYTE>& code)
	{
		ID3D11PixelShader* shader = nullptr;

		device->CreatePixelShader(code.data(), code.size(), nullptr, &shader);
		m_shader.Attach(shader);
	}
}
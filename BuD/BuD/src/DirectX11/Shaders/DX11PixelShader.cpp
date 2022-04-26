#include "DX11PixelShader.h"

namespace BuD
{
	DX11PixelShader::DX11PixelShader(const DX11Device& device, void* code, size_t size)
	{
		if (FAILED(device->CreatePixelShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			printf("Error while creating pixel shader\n");
		}
	}
}
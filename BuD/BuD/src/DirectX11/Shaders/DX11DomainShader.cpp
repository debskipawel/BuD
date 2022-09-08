#include "DX11DomainShader.h"

BuD::DX11DomainShader::DX11DomainShader(const DX11Device& device, void* code, size_t size)
{
	if (FAILED(device->CreateDomainShader(code, size, nullptr, m_shader.GetAddressOf())))
	{
		printf("Error while creating hull shader\n");
	}
}

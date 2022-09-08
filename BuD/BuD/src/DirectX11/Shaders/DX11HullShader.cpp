#include "DX11HullShader.h"

BuD::DX11HullShader::DX11HullShader(const DX11Device& device, void* code, size_t size)
{
	if (FAILED(device->CreateHullShader(code, size, nullptr, m_shader.GetAddressOf())))
	{
		printf("Error while creating hull shader\n");
	}
}

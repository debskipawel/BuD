#include "DX11VertexShader.h"

namespace BuD
{
	DX11VertexShader::DX11VertexShader(ID3D11Device* device, void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{
		if (FAILED(device->CreateVertexShader(code, size, nullptr, m_shader.GetAddressOf())))
		{
			printf("Error while creating vertex shader\n");
		}
		
		if (FAILED(device->CreateInputLayout(inputDesc.data(), static_cast<UINT>(inputDesc.size()), code, size, m_layout.GetAddressOf())))
		{
			printf("Error while creating vertex shader input layout\n");
		}
	}
}
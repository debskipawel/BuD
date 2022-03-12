#include "DX11VertexShader.h"

namespace BuD
{
	DX11VertexShader::DX11VertexShader(ID3D11Device* device, void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{
		ID3D11VertexShader* shader = nullptr;

		if (FAILED(device->CreateVertexShader(code, size, nullptr, &shader)))
		{
			printf("Error while creating vertex shader\n");
		}

		m_shader.Attach(shader);

		ID3D11InputLayout* layout = nullptr;
		
		if (FAILED(device->CreateInputLayout(inputDesc.data(), static_cast<UINT>(inputDesc.size()), code, size, &layout)))
		{
			printf("Error while creating vertex shader input layout\n");
		}

		m_layout.Attach(layout);
	}
}
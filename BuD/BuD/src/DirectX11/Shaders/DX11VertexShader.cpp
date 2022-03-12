#include "DX11VertexShader.h"

namespace BuD
{
	DX11VertexShader::DX11VertexShader(ID3D11Device* device, void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc)
	{
		ID3D11VertexShader* shader = nullptr;
		auto hr = device->CreateVertexShader(code, size, nullptr, &shader);

		m_shader.Attach(shader);

		ID3D11InputLayout* layout = nullptr;
		hr = device->CreateInputLayout(inputDesc.data(), inputDesc.size(), code, size, &layout);

		m_layout.Attach(layout);
	}
}
#pragma once

#include <vector>
#include <d3d11.h>
#include <wrl/client.h>

#include "DX11Shader.h"

using namespace Microsoft::WRL;

namespace BuD
{
	class DX11VertexShader : public DX11Shader
	{
	public:
		explicit DX11VertexShader(ID3D11Device* device, void* code, size_t size, const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputDesc);

		inline ID3D11VertexShader* Shader() { return m_shader.Get(); }
		inline ID3D11InputLayout* Layout() { return m_layout.Get(); }

	private:
		ComPtr<ID3D11InputLayout> m_layout;
		ComPtr<ID3D11VertexShader> m_shader;
	};
}
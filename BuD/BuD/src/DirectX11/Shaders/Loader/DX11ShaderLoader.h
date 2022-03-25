#pragma once

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"
#include "DirectX11/Shaders/DX11GeometryShader.h"

#include <map>
#include <string>

namespace BuD
{
	class DX11ShaderLoader
	{
	public:

		inline static std::shared_ptr<DX11ShaderLoader> Get()
		{
			if (!s_instance)
			{
				s_instance = std::shared_ptr<DX11ShaderLoader>(new DX11ShaderLoader());
			}

			return s_instance;
		}

		std::shared_ptr<DX11VertexShader> VSLoad(ID3D11Device* device, std::wstring shaderPath, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, std::string mainFunName = "main");
		std::shared_ptr<DX11PixelShader> GSLoad(ID3D11Device* device, std::wstring shaderPath, std::string mainFunName = "main");
		std::shared_ptr<DX11PixelShader> PSLoad(ID3D11Device* device, std::wstring shaderPath, std::string mainFunName = "main");

	protected:
		DX11ShaderLoader() = default;

		static std::vector<BYTE> LoadByteCode(const std::wstring& filename);

		static std::shared_ptr<DX11ShaderLoader> s_instance;

		std::map<std::wstring, std::shared_ptr<DX11VertexShader>> m_vertexShaders;
		std::map<std::wstring, std::shared_ptr<DX11PixelShader>> m_pixelShaders;
		std::map<std::wstring, std::shared_ptr<DX11GeometryShader>> m_geometryShaders;
	};
}

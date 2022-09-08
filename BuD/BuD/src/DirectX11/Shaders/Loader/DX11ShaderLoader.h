#pragma once

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"
#include "DirectX11/Shaders/DX11GeometryShader.h"
#include "DirectX11/Shaders/DX11HullShader.h"
#include "DirectX11/Shaders/DX11DomainShader.h"

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

		std::shared_ptr<DX11VertexShader> VSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const std::vector<size_t>& constants = {}, std::string mainFunName = "main");
		std::shared_ptr<DX11HullShader> HSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants = {}, std::string mainFunName = "main");
		std::shared_ptr<DX11DomainShader> DSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants = {}, std::string mainFunName = "main");
		std::shared_ptr<DX11GeometryShader> GSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants = {}, std::string mainFunName = "main");
		std::shared_ptr<DX11PixelShader> PSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants = {}, std::string mainFunName = "main");

	protected:
		DX11ShaderLoader() = default;

		static std::shared_ptr<DX11ShaderLoader> s_instance;

		std::map<std::wstring, std::shared_ptr<DX11VertexShader>> m_vertexShaders;
		std::map<std::wstring, std::shared_ptr<DX11PixelShader>> m_pixelShaders;
		std::map<std::wstring, std::shared_ptr<DX11HullShader>> m_hullShaders;
		std::map<std::wstring, std::shared_ptr<DX11DomainShader>> m_domainShaders;
		std::map<std::wstring, std::shared_ptr<DX11GeometryShader>> m_geometryShaders;
	};
}

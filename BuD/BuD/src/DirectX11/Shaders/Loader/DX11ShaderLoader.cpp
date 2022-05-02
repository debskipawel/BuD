#include "DX11ShaderLoader.h"

#include <iostream>
#include <fstream>

#include <filesystem>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace BuD
{
	std::shared_ptr<DX11ShaderLoader> DX11ShaderLoader::s_instance = nullptr;

	std::vector<BYTE> LoadByteCode(const std::wstring& shaderPath)
	{
		std::wcout << L"Trying to read shader from: " << shaderPath << std::endl;

		std::ifstream sIn(shaderPath, std::ios::in | std::ios::binary);

		sIn.seekg(0, std::ios::end);
		auto byteCodeLength = sIn.tellg();
		sIn.seekg(0, std::ios::beg);

		std::vector<BYTE> byteCode(static_cast<unsigned int>(byteCodeLength));

		sIn.read(reinterpret_cast<char*>(byteCode.data()), byteCodeLength);
		sIn.close();

		return byteCode;
	}

	bool SaveToFile(const void* data, size_t size, const std::wstring& path)
	{
		try
		{
			std::ofstream sOut(path, std::ios::out | std::ios::binary);
			sOut.write(reinterpret_cast<const char*>(data), size);
			sOut.close();

			return true;
		}
		catch (std::exception)
		{
			return false;
		}
	}

	HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
	{
		if (!srcFile || !entryPoint || !profile || !blob)
			return E_INVALIDARG;

		*blob = nullptr;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;
#endif

		const D3D_SHADER_MACRO defines[] =
		{
			"EXAMPLE_DEFINE", "1",
			NULL, NULL
		};

		ID3DBlob* shaderBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;
		HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, profile,
			flags, 0, &shaderBlob, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
				errorBlob->Release();
			}

			if (shaderBlob)
				shaderBlob->Release();

			return hr;
		}

		*blob = shaderBlob;

		return hr;
	}

	template<typename T>
	std::shared_ptr<T> ShaderLoad(
		std::map<std::wstring, std::shared_ptr<T>>& map,
		const DX11Device& device,
		std::wstring shaderPath,
		const std::vector<size_t>& constants,
		std::string mainFunName,
		std::string shaderVersion
	)
	{
		auto result = map.find(shaderPath);
		
		if (result != map.end())
		{
			return result->second;
		}

		std::shared_ptr<T> shader;

		if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			shader = std::make_shared<T>(device, bytecode.data(), bytecode.size());
		}
		else
		{
			auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
			auto compiledShaderName = shaderName + L".cso";

			try
			{
				// try loading compiled shader if exists
				auto bytecode = LoadByteCode(compiledShaderName);
				shader = std::make_shared<T>(device, bytecode.data(), bytecode.size());
			}
			catch (std::exception e)
			{
				ID3DBlob* blob = nullptr;
				auto hr = CompileShader(shaderPath.c_str(), mainFunName.c_str(), shaderVersion.c_str(), &blob);

				shader = std::make_shared<T>(device, blob->GetBufferPointer(), blob->GetBufferSize());

				SaveToFile(blob->GetBufferPointer(), blob->GetBufferSize(), compiledShaderName);

				blob->Release();
			}
		}

		for (auto& size : constants)
		{
			auto cb = std::make_shared<DX11ConstantBuffer>(device, size);
			shader->AddConstantBuffer(cb);
		}

		map.insert(std::make_pair(shaderPath, shader));

		return shader;
	}

	std::shared_ptr<DX11VertexShader> DX11ShaderLoader::VSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, const std::vector<size_t>& constants, std::string mainFunName)
	{
		auto result = m_vertexShaders.find(shaderPath);
		
		if (result != m_vertexShaders.end())
		{
			return result->second;
		}

		std::shared_ptr<DX11VertexShader> shader;

		if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			shader = std::make_shared<DX11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
		}
		else
		{
			auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
			auto compiledShaderName = shaderName + L".cso";

			try
			{
				auto bytecode = LoadByteCode(compiledShaderName);
				shader = std::make_shared<DX11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
			}
			catch (std::exception e)
			{
				ID3DBlob* vsBlob = nullptr;
				auto hr = CompileShader(shaderPath.c_str(), mainFunName.c_str(), "vs_4_0_level_9_1", &vsBlob);
				shader = std::make_shared<DX11VertexShader>(device, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), layout);
				vsBlob->Release();

				SaveToFile(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), compiledShaderName);
			}
		}

		for (auto& size : constants)
		{
			auto cb = std::make_shared<DX11ConstantBuffer>(device, size);
			shader->AddConstantBuffer(cb);
		}

		m_vertexShaders.insert(std::make_pair(shaderPath, shader));

		return shader;
	}

	std::shared_ptr<DX11GeometryShader> DX11ShaderLoader::GSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(m_geometryShaders, device, shaderPath, constants, mainFunName, "gs_4_0_level_9_1");
	}

	std::shared_ptr<DX11PixelShader> DX11ShaderLoader::PSLoad(const DX11Device& device, std::wstring shaderPath, const std::vector<size_t>& constants, std::string mainFunName)
	{
		return ShaderLoad(m_pixelShaders, device, shaderPath, constants, mainFunName, "ps_4_0_level_9_1");
	}
}

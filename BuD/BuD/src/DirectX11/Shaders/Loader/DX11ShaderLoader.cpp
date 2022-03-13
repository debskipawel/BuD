#include "DX11ShaderLoader.h"

#include <iostream>
#include <fstream>

#include <filesystem>

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

namespace BuD
{
	std::shared_ptr<DX11ShaderLoader> DX11ShaderLoader::s_instance = nullptr;

	std::vector<BYTE> DX11ShaderLoader::LoadByteCode(const std::wstring& shaderPath)
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

	std::shared_ptr<DX11VertexShader> DX11ShaderLoader::VSLoad(ID3D11Device* device, std::wstring shaderPath, const std::vector< D3D11_INPUT_ELEMENT_DESC>& layout)
	{
		if (VSLoaded(shaderPath))
		{
			return m_vertexShaders.find(shaderPath)->second;
		}

		if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
		{
			auto bytecode = LoadByteCode(shaderPath);
			return std::make_shared<DX11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
		}

		auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
		auto compiledShaderName = shaderName + L".cso";

		try
		{
			auto bytecode = LoadByteCode(compiledShaderName);
			auto shader = std::make_shared<DX11VertexShader>(device, bytecode.data(), bytecode.size(), layout);
			
			m_vertexShaders.insert(std::make_pair(shaderPath, shader));

			return shader;
		}
		catch (std::exception e) {}

		ID3DBlob* vsBlob = nullptr;
		auto hr = CompileShader(shaderPath.c_str(), "main", "vs_4_0_level_9_1", &vsBlob);

		auto shader = std::make_shared<DX11VertexShader>(device, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), layout);
		m_vertexShaders.insert(std::make_pair(shaderPath, shader));

		vsBlob->Release();

		SaveToFile(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), compiledShaderName);

		return shader;
	}

	std::shared_ptr<DX11PixelShader> DX11ShaderLoader::PSLoad(ID3D11Device* device, std::wstring shaderPath)
	{
		if (PSLoaded(shaderPath))
		{
			return m_pixelShaders.find(shaderPath)->second;
		}

		if (shaderPath.substr(shaderPath.length() - 4) == L".cso")
		{
			// try to load byte code if exists
			auto bytecode = LoadByteCode(shaderPath);
			return std::make_shared<DX11PixelShader>(device, bytecode.data(), bytecode.size());
		}

		auto shaderName = shaderPath.substr(0, shaderPath.find_last_of(L'.'));
		auto compiledShaderName = shaderName + L".cso";

		try
		{
			// try loading compiled shader if exists
			auto bytecode = LoadByteCode(compiledShaderName);
			auto shader = std::make_shared<DX11PixelShader>(device, bytecode.data(), bytecode.size());

			m_pixelShaders.insert(std::make_pair(shaderPath, shader));

			return shader;
		}
		catch (std::exception e) {}

		ID3DBlob* psBlob = nullptr;
		auto hr = CompileShader(shaderPath.c_str(), "main", "ps_4_0_level_9_1", &psBlob);
		
		auto shader = std::make_shared<DX11PixelShader>(device, psBlob->GetBufferPointer(), psBlob->GetBufferSize());
		m_pixelShaders.insert(std::make_pair(shaderPath, shader));

		SaveToFile(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), compiledShaderName);

		psBlob->Release();

		return shader;
	}
}

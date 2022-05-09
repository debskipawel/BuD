#include "DX11StereoscopicRenderer.h"

#include <SimpleMath.h>

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	static std::vector<Vector2> vertices =
	{
		{ -1.0f, -1.0f },
		{  1.0f, -1.0f },
		{ -1.0f,  1.0f },
		{  1.0f,  1.0f },
	};

	static std::vector<unsigned short> indices = { 0, 2, 1, 2, 3, 1 };

	static std::vector<D3D11_INPUT_ELEMENT_DESC> elements
	{
		{
			"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};

	DX11StereoscopicRenderer::DX11StereoscopicRenderer(DX11Device device, std::shared_ptr<Win32Window> window)
		: DX11Renderer(device, window)
	{
		auto width = window->Width();
		auto height = window->Height();

		auto sd = SamplerDescription();
		sd.AddressU = sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.BorderColor[0] = 0.0f;
		sd.BorderColor[1] = 0.0f;
		sd.BorderColor[2] = 0.0f;
		sd.BorderColor[3] = 0.0f;
		auto hr = m_device->CreateSamplerState(&sd, m_sampler.GetAddressOf());

		m_blendVertexShader = DX11ShaderLoader::Get()->VSLoad(m_device, L"../BuD/shaders/f2_to_f4_no_transf_vs.hlsl", elements);
		m_blendPixelShader = DX11ShaderLoader::Get()->PSLoad(m_device, L"../BuD/shaders/two_tex_blend_ps.hlsl");

		m_blendVertexBuffer = std::make_shared<DX11VertexBuffer>(m_device, vertices.size() * sizeof(Vector2), elements, vertices.data());
		m_blendIndexBuffer = std::make_shared<DX11IndexBuffer>(m_device, DXGI_FORMAT_R16_UINT, indices.size() * sizeof(unsigned short), indices.data());

		auto texDesc = DX11Texture2DDesc(width, height);
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_leftSRVTexture.GetAddressOf());
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_rightSRVTexture.GetAddressOf());

		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_leftRTVTexture.GetAddressOf());
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_rightRTVTexture.GetAddressOf());

		hr = m_device->CreateShaderResourceView(m_leftSRVTexture.Get(), nullptr, m_leftEyeSRV.GetAddressOf());
		hr = m_device->CreateShaderResourceView(m_rightSRVTexture.Get(), nullptr, m_rightEyeSRV.GetAddressOf());

		m_leftEyeRTV = m_device.CreateRenderTargetView(m_leftRTVTexture);
		m_rightEyeRTV = m_device.CreateRenderTargetView(m_rightRTVTexture);

		m_leftEyeDepthBuffer = m_device.CreateDepthStencilBuffer(width, height);
		m_rightEyeDepthBuffer = m_device.CreateDepthStencilBuffer(width, height);
	}

	void DX11StereoscopicRenderer::UpdateBuffersSize(int width, int height)
	{
		m_leftSRVTexture.Reset();
		m_leftEyeSRV.Reset();
		m_leftEyeRTV.Reset();
		m_leftEyeDepthBuffer.Reset();
		m_rightSRVTexture.Reset();
		m_rightEyeSRV.Reset();
		m_rightEyeRTV.Reset();
		m_rightEyeDepthBuffer.Reset();

		DX11Renderer::UpdateBuffersSize(width, height);
	}

	void DX11StereoscopicRenderer::Begin()
	{
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		FLOAT nullcolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

		m_device.Context()->ClearRenderTargetView(m_leftEyeRTV.Get(), color);
		m_device.Context()->ClearRenderTargetView(m_rightEyeRTV.Get(), color);
		m_device.Context()->ClearDepthStencilView(m_leftEyeDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_device.Context()->ClearDepthStencilView(m_rightEyeDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		DX11Renderer::Begin();
	}

	void DX11StereoscopicRenderer::Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera, uint32_t id)
	{
		// render left eye
		SetupEntity(entity, camera->GetViewMatrix(), camera->GetLeftEyeProjection());

		m_device.Context()->OMSetRenderTargets(1, m_leftEyeRTV.GetAddressOf(), nullptr);
		m_device.Context()->DrawIndexed(entity->IndexBuffer()->Count(), 0, 0);

		RenderId(entity, id);

		// render right eye
		SetupEntity(entity, camera->GetViewMatrix(), camera->GetRightEyeProjection());

		m_device.Context()->OMSetRenderTargets(1, m_rightEyeRTV.GetAddressOf(), nullptr);
		m_device.Context()->DrawIndexed(entity->IndexBuffer()->Count(), 0, 0);

		RenderId(entity, id);

		m_device.Context()->OMSetRenderTargets(1, m_mainRTV.GetAddressOf(), m_depthBuffer.Get());
	}

	void DX11StereoscopicRenderer::End()
	{
		m_device.Context()->CopyResource(m_leftSRVTexture.Get(), m_leftRTVTexture.Get());
		m_device.Context()->CopyResource(m_rightSRVTexture.Get(), m_rightRTVTexture.Get());

		// use the 2 channels as SRVs and blend them on a (0,0) to (1,1) square 
		m_device.Context()->OMSetRenderTargets(1, m_mainRTV.GetAddressOf(), m_depthBuffer.Get());

		// set shaders
		m_device.Context()->VSSetShader(m_blendVertexShader->Shader(), nullptr, 0);
		m_device.Context()->GSSetShader(nullptr, nullptr, 0);
		m_device.Context()->PSSetShader(m_blendPixelShader->Shader(), nullptr, 0);

		// set textures and samplers
		auto shaderResources = { m_leftEyeSRV.Get(), m_rightEyeSRV.Get() };
		m_device.Context()->PSSetShaderResources(0, shaderResources.size(), shaderResources.begin());
		m_device.Context()->PSSetSamplers(0, 1, m_sampler.GetAddressOf());

		m_device.Context()->IASetInputLayout(m_blendVertexShader->GetLayout());
		m_device.Context()->IASetPrimitiveTopology(m_blendIndexBuffer->Topology());

		m_device.Context()->VSSetConstantBuffers(0, 0, nullptr);
		m_device.Context()->PSSetConstantBuffers(0, 0, nullptr);
		m_device.Context()->GSSetConstantBuffers(0, 0, nullptr);

		ID3D11Buffer* buffers[] = { m_blendVertexBuffer->Buffer() };
		UINT strides[] = { m_blendVertexBuffer->Stride() };
		UINT offsets[] = { m_blendVertexBuffer->Offset() };

		m_device.Context()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		m_device.Context()->IASetIndexBuffer(m_blendIndexBuffer->Buffer(), m_blendIndexBuffer->Format(), 0);

		m_device.Context()->RSSetState(m_backCullSolidState.Get());
		m_device.Context()->DrawIndexed(m_blendIndexBuffer->Count(), 0, 0);
	}

	void DX11StereoscopicRenderer::InitializeBuffers(int width, int height)
	{
		auto texDesc = DX11Texture2DDesc(width, height);
		auto hr = m_device->CreateTexture2D(&texDesc, nullptr, m_leftSRVTexture.GetAddressOf());
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_rightSRVTexture.GetAddressOf());

		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_leftRTVTexture.GetAddressOf());
		hr = m_device->CreateTexture2D(&texDesc, nullptr, m_rightRTVTexture.GetAddressOf());

		hr = m_device->CreateShaderResourceView(m_leftSRVTexture.Get(), nullptr, m_leftEyeSRV.GetAddressOf());
		hr = m_device->CreateShaderResourceView(m_rightSRVTexture.Get(), nullptr, m_rightEyeSRV.GetAddressOf());

		m_leftEyeRTV = m_device.CreateRenderTargetView(m_leftRTVTexture);
		m_rightEyeRTV = m_device.CreateRenderTargetView(m_rightRTVTexture);

		m_leftEyeDepthBuffer = m_device.CreateDepthStencilBuffer(width, height);
		m_rightEyeDepthBuffer = m_device.CreateDepthStencilBuffer(width, height);

		DX11Renderer::InitializeBuffers(width, height);
	}
}

#ifdef _WIN32

#include "DX11Renderer.h"

#include "DX11Structures.h"

#include <algorithm>

namespace BuD
{
	BuD::DX11Renderer::DX11Renderer(std::shared_ptr<Win32Window> window)
		: m_device(window)
	{
		ComPtr<ID3D11Texture2D> backTexture;
		auto hr = m_device.SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTexture.GetAddressOf());

		auto width = window->Width();
		auto height = window->Height();

		m_backBuffer = m_device.CreateRenderTargetView(backTexture);
		m_depthBuffer = m_device.CreateDepthStencilBuffer(width, height);
		backTexture.Reset();

		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());

		DX11Viewport viewport{ (UINT)width, (UINT)height };
		m_device.Context()->RSSetViewports(1, &viewport);
	}

	void DX11Renderer::UpdateBuffersSize(int width, int height)
	{
		m_device.Context()->OMSetRenderTargets(0, nullptr, nullptr);

		m_backBuffer.Reset();
		m_depthBuffer.Reset();

		m_device.Context()->Flush();

		auto hr = m_device.SwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		ComPtr<ID3D11Texture2D> backTexture;
		m_device.SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTexture.GetAddressOf());

		m_backBuffer = m_device.CreateRenderTargetView(backTexture);
		m_depthBuffer = m_device.CreateDepthStencilBuffer(width, height);

		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());

		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		backTexture->GetDesc(&backBufferDesc);

		DX11Viewport viewport{ backBufferDesc.Width, backBufferDesc.Height };
		m_device.Context()->RSSetViewports(1, &viewport);
		
		backTexture.Reset();
	}

	void DX11Renderer::Begin()
	{
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_device.Context()->ClearRenderTargetView(m_backBuffer.Get(), color);
		m_device.Context()->ClearDepthStencilView(m_depthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void BuD::DX11Renderer::Draw(std::shared_ptr<RenderableSceneEntity> entity, std::shared_ptr<AbstractCamera> camera)
	{
		m_device.Context()->VSSetShader(entity->m_vertexShader->Shader(), nullptr, 0);
		m_device.Context()->PSSetShader(entity->m_pixelShader->Shader(), nullptr, 0);

		m_device.Context()->IASetInputLayout(entity->m_vertexShader->Layout());
		m_device.Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		if (auto count = entity->m_vertexShader->ConstantBuffers().size())
		{
			auto rawBuffers = entity->m_vertexShader->RawConstantBuffers();
			m_device.Context()->VSSetConstantBuffers(0, count, rawBuffers);
		}

		if (auto count = entity->m_pixelShader->ConstantBuffers().size())
		{
			auto rawBuffers = entity->m_pixelShader->RawConstantBuffers();
			m_device.Context()->PSSetConstantBuffers(0, count, rawBuffers);
		}

		entity->UpdateConstantBuffers(camera);

		ID3D11Buffer* buffers[] = { entity->m_vertexBuffer->Buffer() };
		UINT strides[] = { entity->m_vertexBuffer->Stride() };
		UINT offsets[] = { entity->m_vertexBuffer->Offset() };

		m_device.Context()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		m_device.Context()->IASetIndexBuffer(entity->m_indexBuffer->Buffer(), entity->m_indexBuffer->Format(), 0);

		D3D11_RASTERIZER_DESC wfdesc;
		ID3D11RasterizerState* rastState = nullptr;

		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		m_device->CreateRasterizerState(&wfdesc, &rastState);
		m_device.Context()->RSSetState(rastState);

		m_device.Context()->DrawIndexed(entity->m_indexBuffer->Count(), 0, 0);
	}

	void DX11Renderer::End()
	{
		m_device.SwapChain()->Present(0, 0);
	}
}

#endif

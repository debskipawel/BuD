#ifdef _WIN32

#include "DX11Renderer.h"
#include "DX11Structures.h"

#include "Shaders/Loader/DX11ShaderLoader.h"

#include <algorithm>

namespace BuD
{
	BuD::DX11Renderer::DX11Renderer(std::shared_ptr<Win32Window> window)
		: m_device(window)
	{
		auto width = window->Width();
		auto height = window->Height();

		InitializeBuffers(width, height);
	}

	uint32_t DX11Renderer::GetObjectFrom(int x, int y)
	{
		D3D11_MAPPED_SUBRESOURCE subr;
		ZeroMemory(&subr, sizeof(D3D11_MAPPED_SUBRESOURCE));

		return uint32_t();
	}

	void DX11Renderer::UpdateBuffersSize(int width, int height)
	{
		m_device.Context()->OMSetRenderTargets(0, nullptr, nullptr);

		m_backBuffer.Reset();
		m_idTexture.Reset();
		m_depthBuffer.Reset();
		m_idDepthBuffer.Reset();

		m_device.Context()->Flush();

		auto hr = m_device.SwapChain()->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);

		InitializeBuffers(width, height);
	}

	void DX11Renderer::InitializeBuffers(int width, int height)
	{
		ComPtr<ID3D11Texture2D> backTexture;
		m_device.SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backTexture.GetAddressOf());

		ComPtr<ID3D11Texture2D> idTexture;
		auto texDesc = DX11Texture2DDesc(width, height);
		texDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		auto hr = m_device->CreateTexture2D(&texDesc, nullptr, idTexture.GetAddressOf());

		m_backBuffer = m_device.CreateRenderTargetView(backTexture);
		m_idTexture = m_device.CreateRenderTargetView(idTexture);
		m_depthBuffer = m_device.CreateDepthStencilBuffer(width, height);
		m_idDepthBuffer = m_device.CreateDepthStencilBuffer(width, height);

		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());

		D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
		backTexture->GetDesc(&backBufferDesc);

		DX11Viewport viewport{ backBufferDesc.Width, backBufferDesc.Height };
		m_device.Context()->RSSetViewports(1, &viewport);

		backTexture.Reset();
		idTexture.Reset();
	}

	void DX11Renderer::Begin()
	{
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		FLOAT nullcolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		
		m_device.Context()->ClearRenderTargetView(m_backBuffer.Get(), color);
		m_device.Context()->ClearRenderTargetView(m_idTexture.Get(), nullcolor);
		m_device.Context()->ClearDepthStencilView(m_depthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_device.Context()->ClearDepthStencilView(m_idDepthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	void BuD::DX11Renderer::Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera)
	{
		// draw to the backbuffer
		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());

		m_device.Context()->VSSetShader(entity->m_vertexShader->Shader(), nullptr, 0);
		m_device.Context()->PSSetShader(entity->m_pixelShader->Shader(), nullptr, 0);

		m_device.Context()->IASetInputLayout(entity->m_vertexShader->Layout());
		m_device.Context()->IASetPrimitiveTopology(entity->IndexBuffer()->Topology());

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

		// draw to the texture contatining id
		m_device.Context()->OMSetRenderTargets(1, m_idTexture.GetAddressOf(), m_idDepthBuffer.Get());

		// set pixel shader to something containing id as constant buffer
		m_device.Context()->PSSetShader(GetIdShader(m_device)->Shader(), nullptr, 0);

		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_NONE;
		m_device->CreateRasterizerState(&wfdesc, &rastState);
		m_device.Context()->RSSetState(rastState);

		m_device.Context()->DrawIndexed(entity->m_indexBuffer->Count(), 0, 0);

		// return to backbuffer for gui render
		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());
	}

	void DX11Renderer::End()
	{
		m_device.SwapChain()->Present(0, 0);
	}

	std::shared_ptr<DX11PixelShader> DX11Renderer::GetIdShader(const DX11Device& device)
	{
		if (!s_idShader)
		{
			s_idShader = DX11ShaderLoader::Get()->PSLoad(device.Raw(), L"../BuD/shaders/render_id_ps.hlsl");
			s_idShader->AddConstantBuffer(std::make_shared<DX11ConstantBuffer>(device, 16));
		}

		return s_idShader;
	}

	std::shared_ptr<DX11PixelShader> DX11Renderer::s_idShader = nullptr;
}

#endif

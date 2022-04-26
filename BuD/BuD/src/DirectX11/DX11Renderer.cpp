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

		D3D11_RASTERIZER_DESC wfdesc;

		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_WIREFRAME;
		wfdesc.CullMode = D3D11_CULL_NONE;
		m_device->CreateRasterizerState(&wfdesc, m_noCullWireframeState.GetAddressOf());

		wfdesc.CullMode = D3D11_CULL_BACK;
		m_device->CreateRasterizerState(&wfdesc, m_backCullWireframeState.GetAddressOf());

		wfdesc.FillMode = D3D11_FILL_SOLID;
		m_device->CreateRasterizerState(&wfdesc, m_backCullSolidState.GetAddressOf());
	}

	uint32_t DX11Renderer::GetObjectFrom(int x, int y)
	{
		D3D11_MAPPED_SUBRESOURCE subr;
		ZeroMemory(&subr, sizeof(D3D11_MAPPED_SUBRESOURCE));

		ComPtr<ID3D11Texture2D> stagingTexture;
		DX11Texture2DDesc texDesc(m_width, m_height);
		texDesc.Usage = D3D11_USAGE_STAGING;
		texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		texDesc.BindFlags = 0;

		m_device->CreateTexture2D(&texDesc, nullptr, stagingTexture.GetAddressOf());

		ID3D11Resource* res = nullptr;
		m_idTexture->GetResource(&res);
		
		m_device.Context()->CopyResource(stagingTexture.Get(), res);
		m_device.Context()->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &subr);

		auto rowPitch = subr.RowPitch / sizeof(int);
		int* data = static_cast<int*>(subr.pData);
		data += y * rowPitch + x;
		
		int result = *data;

		m_device.Context()->Unmap(stagingTexture.Get(), 0);
		stagingTexture.Reset();

		BYTE r = (result & 0x000000ff);
		BYTE g = (result & 0x0000ff00) >> 8;
		BYTE b = (result & 0x00ff0000) >> 16;

		return b + 256 * (g + 256 * r);
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
		m_width = width;
		m_height = height;

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

	void BuD::DX11Renderer::Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera, uint32_t id)
	{
		// draw to the backbuffer
		m_device.Context()->OMSetRenderTargets(1, m_backBuffer.GetAddressOf(), m_depthBuffer.Get());

		m_device.Context()->VSSetShader(entity->m_vertexShader->Shader(), nullptr, 0);
		m_device.Context()->PSSetShader(entity->m_pixelShader->Shader(), nullptr, 0);

		m_device.Context()->IASetInputLayout(entity->m_vertexShader->GetLayout());
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

		if (auto& gs = entity->m_geometryShader)
		{
			m_device.Context()->GSSetShader(gs->Shader(), nullptr, 0);

			auto count = gs->ConstantBuffers().size();
			m_device.Context()->GSSetConstantBuffers(0, count, gs->RawConstantBuffers());
		}
		else
		{
			m_device.Context()->GSSetShader(nullptr, nullptr, 0);
		}

		entity->UpdateConstantBuffers(camera);

		ID3D11Buffer* buffers[] = { entity->m_vertexBuffer->Buffer() };
		UINT strides[] = { entity->m_vertexBuffer->Stride() };
		UINT offsets[] = { entity->m_vertexBuffer->Offset() };

		m_device.Context()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		m_device.Context()->IASetIndexBuffer(entity->m_indexBuffer->Buffer(), entity->m_indexBuffer->Format(), 0);

		m_device.Context()->RSSetState(m_noCullWireframeState.Get());
		m_device.Context()->DrawIndexed(entity->m_indexBuffer->Count(), 0, 0);

		// draw to the texture contatining id
		m_device.Context()->OMSetRenderTargets(1, m_idTexture.GetAddressOf(), m_idDepthBuffer.Get());

		// set pixel shader to something containing id as constant buffer
		auto pixelShader = GetIdShader(m_device);
		pixelShader->UpdateConstantBuffer(0, &id, sizeof(uint32_t));
		
		m_device.Context()->PSSetShader(pixelShader->Shader(), nullptr, 0);

		if (auto count = pixelShader->ConstantBuffers().size())
		{
			auto rawBuffers = pixelShader->RawConstantBuffers();
			m_device.Context()->PSSetConstantBuffers(0, count, rawBuffers);
		}

		m_device.Context()->RSSetState(m_backCullSolidState.Get());
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
			s_idShader = DX11ShaderLoader::Get()->PSLoad(device, L"../BuD/shaders/render_id_ps.hlsl", {});
			s_idShader->AddConstantBuffer(std::make_shared<DX11ConstantBuffer>(device, 16));
		}

		return s_idShader;
	}

	std::shared_ptr<DX11PixelShader> DX11Renderer::s_idShader = nullptr;
}

#endif

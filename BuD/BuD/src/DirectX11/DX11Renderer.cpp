#ifdef _WIN32

#include "DX11Renderer.h"

namespace BuD
{
	BuD::DX11Renderer::DX11Renderer(std::shared_ptr<Win32Window> window)
		: m_device(window)
	{
		ID3D11Texture2D* backBuffer;

		m_device.SwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
		m_device->CreateRenderTargetView(backBuffer, NULL, &m_rtv);

		backBuffer->Release();

		m_device.Context()->OMSetRenderTargets(1, &m_rtv, NULL);
	}

	BuD::DX11Renderer::~DX11Renderer()
	{
		Cleanup();
	}

	void BuD::DX11Renderer::Cleanup()
	{
		if (m_rtv)
		{
			m_rtv->Release();
		}
	}

	void BuD::DX11Renderer::UpdateBufferSize(int width, int height)
	{
	}

	void DX11Renderer::Begin()
	{
		FLOAT color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_device.Context()->ClearRenderTargetView(m_rtv, color);
	}

	void BuD::DX11Renderer::Draw(const RenderableSceneEntity& entity)
	{
		m_device.Context()->VSSetShader(entity.m_vertexShader->Shader(), nullptr, 0);
		m_device.Context()->PSSetShader(entity.m_pixelShader->Shader(), nullptr, 0);

		m_device.Context()->IASetInputLayout(entity.m_vertexShader->Layout());
		m_device.Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		ID3D11Buffer* buffers[] = { entity.m_vertexBuffer->Buffer() };
		UINT strides[] = { entity.m_vertexBuffer->Stride() };
		UINT offsets[] = { entity.m_vertexBuffer->Offset() };

		m_device.Context()->IASetVertexBuffers(0, 1, buffers, strides, offsets);
		m_device.Context()->IASetIndexBuffer(entity.m_indexBuffer->Buffer(), entity.m_indexBuffer->Format(), 0);

		if (auto count = entity.m_vertexShader->ConstantBuffers().size())
		{
			m_device.Context()->VSSetConstantBuffers(0, count, entity.m_vertexShader->RawConstantBuffers());
		}

		if (auto count = entity.m_pixelShader->ConstantBuffers().size())
		{
			m_device.Context()->VSSetConstantBuffers(0, count, entity.m_pixelShader->RawConstantBuffers());
		}

		D3D11_RASTERIZER_DESC wfdesc;
		ID3D11RasterizerState* rastState = nullptr;

		ZeroMemory(&wfdesc, sizeof(D3D11_RASTERIZER_DESC));
		wfdesc.FillMode = D3D11_FILL_SOLID;
		wfdesc.CullMode = D3D11_CULL_NONE;
		m_device->CreateRasterizerState(&wfdesc, &rastState);
		m_device.Context()->RSSetState(rastState);

		m_device.Context()->DrawIndexed(entity.m_indexBuffer->Count(), 0, 0);
	}

	void DX11Renderer::End()
	{
		m_device.SwapChain()->Present(0, 0);
	}
}

#endif

#pragma once

#include "DX11Renderer.h"

#include <DirectX11/Buffers/DX11VertexBuffer.h>
#include <DirectX11/Buffers/DX11IndexBuffer.h>

namespace BuD
{
	class DX11StereoscopicRenderer : public DX11Renderer
	{
	public:
		DX11StereoscopicRenderer(std::shared_ptr<Win32Window> window);

		virtual void UpdateBuffersSize(int width, int height) override;
		virtual void InitializeBuffers(int width, int height) override;

		virtual void Begin() override;
		virtual void Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera, uint32_t id = 0) override;
		virtual void End() override;

	protected:
		ComPtr<ID3D11Texture2D> m_leftSRVTexture;
		ComPtr<ID3D11Texture2D> m_rightSRVTexture;
		ComPtr<ID3D11Texture2D> m_leftRTVTexture;
		ComPtr<ID3D11Texture2D> m_rightRTVTexture;
		ComPtr<ID3D11ShaderResourceView> m_leftEyeSRV;
		ComPtr<ID3D11ShaderResourceView> m_rightEyeSRV;
		ComPtr<ID3D11RenderTargetView> m_leftEyeRTV;
		ComPtr<ID3D11RenderTargetView> m_rightEyeRTV;
		ComPtr<ID3D11DepthStencilView> m_leftEyeDepthBuffer; 
		ComPtr<ID3D11DepthStencilView> m_rightEyeDepthBuffer;

		ComPtr<ID3D11SamplerState> m_sampler;

		std::shared_ptr<DX11VertexShader> m_blendVertexShader;
		std::shared_ptr<DX11PixelShader> m_blendPixelShader;
		std::shared_ptr<DX11VertexBuffer> m_blendVertexBuffer;
		std::shared_ptr<DX11IndexBuffer> m_blendIndexBuffer;
	};
}

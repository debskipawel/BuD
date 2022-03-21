#pragma once

#ifdef _WIN32

#include "Core/Renderer.h"
#include "Win32/Win32Window.h"

#include "DX11Device.h"
#include "Shaders/DX11PixelShader.h"
#include "Buffers/DX11ConstantBuffer.h"

#include <Windows.h>
#include <d3d11_1.h>
#include <memory>

namespace BuD
{
	class DX11Renderer : public Renderer
	{
	public:
		DX11Renderer(std::shared_ptr<Win32Window> window);

		inline const DX11Device& Device() const { return m_device; }

		virtual void UpdateBuffersSize(int width, int height) override;

		virtual void Begin() override;
		virtual void Draw(std::shared_ptr<Mesh> entity, std::shared_ptr<AbstractCamera> camera) override;
		virtual void End() override;

		uint32_t GetObjectFrom(int x, int y);

	private:
		void InitializeBuffers(int width, int height);
		
		DX11Device m_device;
		ComPtr<ID3D11RenderTargetView> m_backBuffer;
		ComPtr<ID3D11RenderTargetView> m_idTexture;
		ComPtr<ID3D11DepthStencilView> m_depthBuffer;
		ComPtr<ID3D11DepthStencilView> m_idDepthBuffer;

		static std::shared_ptr<DX11PixelShader> GetIdShader(const DX11Device&);
		static std::shared_ptr<DX11PixelShader> s_idShader;
	};
}

#endif

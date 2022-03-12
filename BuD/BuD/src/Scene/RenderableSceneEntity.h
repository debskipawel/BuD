#pragma once

#include "SceneEntity.h"

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"

#include "DirectX11/Buffers/DX11VertexBuffer.h"
#include "DirectX11/Buffers/DX11IndexBuffer.h"

namespace BuD
{
	class RenderableSceneEntity : public SceneEntity
	{
	public:
		friend class DX11Renderer;

		static std::shared_ptr<RenderableSceneEntity> Cube(ID3D11Device* device);
	protected:
		RenderableSceneEntity() = default;

		std::shared_ptr<DX11VertexShader> m_vertexShader;
		std::shared_ptr<DX11PixelShader> m_pixelShader;

		std::shared_ptr<DX11VertexBuffer> m_vertexBuffer;
		std::shared_ptr<DX11IndexBuffer> m_indexBuffer;
	};
}

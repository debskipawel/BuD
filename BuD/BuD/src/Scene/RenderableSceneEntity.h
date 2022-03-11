#pragma once

#include "SceneEntity.h"

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"

namespace BuD
{
	class RenderableSceneEntity : public SceneEntity
	{
	public:
		friend class DX11Renderer;
	protected:
		RenderableSceneEntity() = default;

		std::shared_ptr<DX11VertexShader> m_vertexShader;
		std::shared_ptr<DX11PixelShader> m_pixelShader;
	};
}

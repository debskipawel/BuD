#pragma once

#include "SceneEntity.h"

#include "Camera/AbstractCamera.h"

#include "DirectX11/Shaders/DX11VertexShader.h"
#include "DirectX11/Shaders/DX11PixelShader.h"
#include "DirectX11/Buffers/DX11VertexBuffer.h"
#include "DirectX11/Buffers/DX11IndexBuffer.h"

#include <functional>

namespace BuD
{
	class RenderableSceneEntity : public SceneEntity
	{
	public:
		RenderableSceneEntity(
			std::shared_ptr<DX11VertexShader> vertexShader,
			std::shared_ptr<DX11PixelShader>  pixelShader,
			std::shared_ptr<DX11VertexBuffer> vertexBuffer,
			std::shared_ptr<DX11IndexBuffer>  indexBuffer,
			std::function<void(std::shared_ptr<AbstractCamera>, RenderableSceneEntity*)> updateFunc = [](std::shared_ptr<AbstractCamera>, RenderableSceneEntity*) {}
		);

		inline std::shared_ptr<DX11VertexShader> VertexShader() const
		{
			return m_vertexShader;
		}

		inline std::shared_ptr<DX11PixelShader> PixelShader() const
		{
			return m_pixelShader;
		}

		inline std::shared_ptr<DX11VertexBuffer> VertexBuffer() const
		{
			return m_vertexBuffer;
		}

		inline std::shared_ptr<DX11IndexBuffer> IndexBuffer() const
		{
			return m_indexBuffer;
		}

	protected:
		RenderableSceneEntity() = default;

		void UpdateConstantBuffers(std::shared_ptr<AbstractCamera> camera);

		std::shared_ptr<DX11VertexShader> m_vertexShader;
		std::shared_ptr<DX11PixelShader> m_pixelShader;

		std::shared_ptr<DX11VertexBuffer> m_vertexBuffer;
		std::shared_ptr<DX11IndexBuffer> m_indexBuffer;

		std::function<void(std::shared_ptr<AbstractCamera>, RenderableSceneEntity*)> m_updateFunc;

		friend class DX11Renderer;
	};
}

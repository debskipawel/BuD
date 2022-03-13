#include "RenderableSceneEntity.h"

#include <SimpleMath.h>

#include <DirectX11/Shaders/Loader/DX11ShaderLoader.h>

using namespace DirectX::SimpleMath;

namespace BuD
{
	RenderableSceneEntity::RenderableSceneEntity(
		std::shared_ptr<DX11VertexShader> vertexShader, 
		std::shared_ptr<DX11PixelShader> pixelShader, 
		std::shared_ptr<DX11VertexBuffer> vertexBuffer, 
		std::shared_ptr<DX11IndexBuffer> indexBuffer, 
		std::function<void(std::shared_ptr<AbstractCamera>, RenderableSceneEntity*)> updateFunc
	)
		: m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer), m_pixelShader(pixelShader), m_vertexShader(vertexShader), m_updateFunc(updateFunc)
	{
	}

	void RenderableSceneEntity::UpdateConstantBuffers(std::shared_ptr<AbstractCamera> camera)
	{
		m_updateFunc(camera, this);
	}
}
